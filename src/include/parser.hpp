#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include "scope.hpp"
#include "token.hpp"
#include "util.hpp"
#include <iostream>
#include <string>

class Parser {
public:
  Lexer *lexer = nullptr;
  Token *currentToken = nullptr;

  Parser(std::string src) {
    this->lexer = new Lexer(src);
    this->currentToken = this->lexer->nextToken();
  }
  void printLocation() {
    std::cout << "\tLocation: " << (this->lexer->line + 1) << ":"
              << this->lexer->column << std::endl;
  }
  void printError() {
    std::cout << "[PARSER]: Unrecognized Token: \n\t";
    printToken(this->currentToken);
    this->printLocation();
    exit(1);
  }
  Token *eat(token_T type) {
    // std::cout << "[PARSER]: Attempting to Eat Token: ";
    // printToken(this->currentToken);
    if (this->currentToken->type != type) {
      // this->printError();
      std::cout << "[PARSER]: Expected Token Type '" << getTokenType(type)
                << "' But Found Type '"
                << getTokenType(this->currentToken->type) << "'" << std::endl;
      std::cout << "\tCurrent Token: ";
      printToken(this->currentToken);
      this->printLocation();
      exit(1);
    }
    this->currentToken = this->lexer->nextToken();
    return this->currentToken;
  }
  AST *parse(Scope *scope = new Scope(), token_T closer = TOKEN_EOF) {
    AST *root = new AST(AST_ROOT);

    while (this->currentToken->type != closer) {
      // printToken(this->currentToken);
      switch (this->currentToken->type) {
      case TOKEN_ID:
        root->children.push_back(this->parseId(scope));
        break;
      case TOKEN_PRIM_TYPE:
        root->children.push_back(this->parsePrimType(scope));
        break;
      default:
        this->printError();
        exit(1);
      }
    }

    root->scope = scope;

    return root;
  }
  AST *parseId(Scope *scope) {
    AST *ret = new AST(AST_NULL);

    if (this->currentToken->value == "import") {
      // import statement
      ret->type = AST_IMPORT;
      this->eat(TOKEN_ID);
      ret->value = this->currentToken->strValue;
      this->eat(TOKEN_STRING);
    } else if (this->currentToken->value == "extern") {
      // extern statement
      ret->type = AST_EXTERN;
      this->eat(TOKEN_ID);
      ret->externLang = this->currentToken->value;
      this->eat(TOKEN_ID);
      this->eat(TOKEN_EQUALS);
      ret->value = this->currentToken->strValue;
      ret->value.pop_back();
      ret->value.erase(ret->value.begin());
      this->eat(TOKEN_STRING);
      this->eat(TOKEN_SEMICOLON);
    } else if (this->currentToken->type == TOKEN_ID) {
      // function call or assigning a variable or recalling a variable
      ret->value = this->currentToken->value;
      this->eat(TOKEN_ID);
      if (this->currentToken->type == TOKEN_LPAREN) {
        // function call
        ret->type = AST_FUNCTION_CALL;
        this->eat(TOKEN_LPAREN);
        ret->arguments = this->parseArguments(scope);
        this->eat(TOKEN_RPAREN);
        this->eat(TOKEN_SEMICOLON);
      } else if (this->currentToken->type == TOKEN_EQUALS) {
        // TODO: variable reassignment
      } else {
        ret->type = AST_VARIABLE_RECALL;
      }

    } else {
      this->printError();
      exit(1);
    }

    return ret;
  }
  AST *parsePrimType(Scope *scope) {
    AST *ret = new AST(AST_DECLARATION);

    ret->declType = this->currentToken->value;
    this->eat(TOKEN_PRIM_TYPE);
    ret->declarator = new AST(AST_NULL);
    ret->declarator->value = this->currentToken->value;
    this->eat(TOKEN_ID);

    if (this->currentToken->type == TOKEN_LPAREN) {
      // function declaration
      ret->declarator->type = AST_FUNCTION_DECLARATION;
      this->eat(TOKEN_LPAREN);
      ret->declarator->arguments = this->parseParameters(scope);
      this->eat(TOKEN_RPAREN);

      this->eat(TOKEN_LBRACE);
      ret->declarator->body = this->parseCompound(scope);
      this->eat(TOKEN_RBRACE);
    } else {
      // variable initialization
      ret->type = AST_VARIABLE_ASSIGNMENT;
      this->eat(TOKEN_EQUALS);
      if (ret->declType == "str") {
        // std::cout << ret->declarator->value << std::endl;

        /*
            str myString = "Hello, World!";
                |
                V
            AST {
                declType = "str"
                value = "Hello, World!"
                declarator {
                    value = "myString"
                }
            }
        */

        ret->value = this->currentToken->strValue;
        this->eat(TOKEN_STRING);
        this->eat(TOKEN_SEMICOLON);

        scope->addNewVariable(ret->declType, ret->declarator->value);

      } else if (ret->declType == "int") {
        // parse expression
        ret->body = this->parseExpression(scope);
        this->eat(TOKEN_SEMICOLON);

        // std::cout << "I have an integer variable with a value of "
        //           << ret->body->value << std::endl;
        scope->addNewVariable(ret->declType, ret->declarator->value,
                              ret->body->value);
      }
    }

    return ret;
  }
  std::vector<AST *> parseParameters(Scope *scope) {
    std::vector<AST *> ret;

    while (this->currentToken->type != TOKEN_RPAREN) {
      AST *param = new AST(AST_PARAMETER);
      param->declType = this->currentToken->value;
      this->eat(TOKEN_PRIM_TYPE);
      param->value = this->currentToken->value;
      this->eat(TOKEN_ID);
      ret.push_back(param);
      if (this->currentToken->type != TOKEN_RPAREN)
        this->eat(TOKEN_COMMA);
    }

    return ret;
  }
  std::vector<AST *> parseArguments(Scope *scope) {
    std::vector<AST *> ret;

    // std::cout << std::endl;

    while (this->currentToken->type != TOKEN_RPAREN) {
      // printToken(this->currentToken);
      AST *param = this->parseArg(scope);
      ret.push_back(param);
      if (this->currentToken->type != TOKEN_RPAREN)
        this->eat(TOKEN_COMMA);
    }

    return ret;
  }
  AST *parseArg(Scope *scope) {
    AST *ret = new AST(AST_ARGUMENT);

    if (this->currentToken->type == TOKEN_STRING) {
      ret->declType = "str";
      ret->value = this->currentToken->strValue;
      this->eat(TOKEN_STRING);
    } else if (this->currentToken->type == TOKEN_ID) {
      ret->type = AST_VARIABLE;
      ret->value = this->currentToken->value;
      this->eat(TOKEN_ID);
    } else if (this->currentToken->type == TOKEN_INT) {
      ret->body = this->parseExpression(scope);
      ret->type = AST_EXPRESSION;
    }

    return ret;
  }
  AST *parseCompound(Scope *scope) {
    AST *parsed = this->parse(scope, TOKEN_RBRACE);

    parsed->type = AST_COMPOUND;

    return parsed;
  }
  AST *parseExpression(Scope *scope) {
    AST *ret = new AST(AST_EXPRESSION);

    AST *left;
    if (this->currentToken->type == TOKEN_ID) {
      left = this->parseId(scope);
    } else {
      left = this->parsePrimaryExpression(scope);
    }
    if (isBinaryOperator(this->currentToken->type)) {
      op_T operation = this->parseOperation(scope);
      ret->left = left;
      ret->binOp = operation;
      ret->right = this->parseExpression(scope);
    } else if (left->type == AST_EXPRESSION_PRIMARY) {
      ret = left;
    }

    // std::cout << "LEFT: " << ret->left << std::endl;
    // std::cout << "BINOP: " << ret->binOp << std::endl;
    // std::cout << "RIGHT: " << ret->right << std::endl;

    return ret;
  }
  AST *parsePrimaryExpression(Scope *scope) {
    AST *ret = new AST(AST_EXPRESSION_PRIMARY);

    ret->value = std::to_string(this->currentToken->intValue);
    this->eat(TOKEN_INT);

    return ret;
  }
  op_T parseOperation(Scope *scope) {
    op_T ret;

    switch (this->currentToken->type) {
    case TOKEN_PLUS:
      ret = OP_ADD;
      break;
    case TOKEN_MINUS:
      ret = OP_SUB;
      break;
    case TOKEN_STAR:
      ret = OP_MUL;
      break;
    case TOKEN_SLASH:
      ret = OP_DIV;
      break;
    default:
      this->printError();
      exit(1);
    }

    this->eat(this->currentToken->type);

    return ret;
  }
};
