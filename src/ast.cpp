#include "ast.hpp"
#include <iostream>
#include <string>

void printAST(AST *ast, std::string tabs, int finalReturn = 1) {
  if (ast == NULL)
    return;
  std::string lTabs = tabs;
  if (ast->type)
    std::cout << lTabs << getASTType(ast->type) << " ";
  std::cout << "value: " << ast->value << " ";
  std::cout << "externLang: " << ast->externLang << " ";
  std::cout << "declType: " << ast->declType << " ";
  std::cout << "declarator: ";
  printAST(ast->declarator, "", 0);
  std::cout << "body: ";
  printAST(ast->body, "", 0);
  std::cout << "left: ";
  printAST(ast->left, "", 0);
  std::cout << "right: ";
  printAST(ast->right, "", 0);
  std::cout << "binOp: " << ast->binOp << " ";
  lTabs += '\t';
  std::cout << "arguments: ";
  for (auto &a : ast->arguments) {
    printAST(a, lTabs);
  }
  std::cout << "children: ";
  for (auto &c : ast->children) {
    printAST(c, lTabs);
  }
  if (finalReturn)
    std::cout << std::endl;
}

void printAST(AST *ast) { printAST(ast, ""); }

std::string getASTType(ast_T type) {
  const static std::string names[] = {
      "AST_ROOT",
      "AST_IMPORT",
      "AST_EXTERN",
      "AST_WHILE",
      "AST_FOR",
      "AST_IF",
      "AST_FUNCTION_DECLARATION",
      "AST_FUNCTION_CALL",
      "AST_ARGUMENT",
      "AST_COMPOUND",
      "AST_NULL",
      "AST_TYPE",
      "AST_DECLARATION",
      "AST_PARAMETER",
      "AST_EXPRESSION",
      "AST_VARIABLE_ASSIGNMENT",
      "AST_VARIABLE_RECALL",
      "AST_VARIABLE",
      "AST_EXPRESSION_PRIMARY",
  };
  return names[type];
}

std::string getOPType(op_T op) {
  const static std::string names[] = {
      "OP_ADD", "OP_SUB",    "OP_DIV", "OP_MUL",  "OP_LT",
      "OP_GT",  "OP_ASSIGN", "OP_CMP", "OP_LTET", "OP_GTET",
  };
  return names[op];
}
