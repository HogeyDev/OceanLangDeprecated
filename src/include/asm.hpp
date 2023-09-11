#include "ast.hpp"
#include "io.hpp"
#include "ocean.hpp"
#include "parser.hpp"
#include "scope.hpp"
#include <iostream>
#include <string>
#include <vector>

class AsmFrontend {
public:
  AST *root;
  std::vector<std::string> pragmaFilePaths;
  std::vector<std::vector<std::string>> strings;

  AsmFrontend(AST *root) { this->root = root; }
  std::string addNewString(std::string value) {
    std::vector<std::string> row;
    for (auto &s : this->strings) {
      if (s[1] != value)
        continue;
      return s[0];
    }
    row.push_back("STR" + std::to_string(this->strings.size()));
    row.push_back(value);
    this->strings.push_back(row);
    return row[0];
  }
  std::string compileRootCompound(AST *ast, Scope *scope, int ismain = 0) {
    std::string ret;
    // if (ismain) ret += "%define main _start\n";

    for (auto &child : ast->children) {
      switch (child->type) {
      case AST_IMPORT:
        ret += this->compileImport(child, scope) + "\n";
        break;
      case AST_EXTERN:
        ret += this->compileExtern(child, scope) + "\n";
        break;
      case AST_DECLARATION:
        ret += this->compileDeclaration(child, scope) + "\n";
        break;
      case AST_FUNCTION_CALL:
        ret += this->compileFunctionCall(child, scope) + "\n";
        break;
      case AST_VARIABLE_ASSIGNMENT:
        ret += this->compileVariableAssignment(child, scope) + "\n";
        break;
      default: {
        std::cout << "Unknown AST: " << getASTType(child->type) << std::endl;
        exit(1);
      }
      }
    }
    if (ismain > 0)
      ret += "\n" + this->generateDataSection();

    return ret;
  }
  std::string compileVariableAssignment(AST *ast, Scope *scope) {
    std::string ret;
    if (ast->declType == "str") {
      std::string pushed = this->addNewString(ast->value);
      ret += "sub rsp, 8\n";
      ret += "mov QWORD [rsp+8], " + pushed + "\n";
    } else if (ast->declType == "int") {
      ret += this->compileExpression(ast->body, scope);
    }
    // std::cout << scope->getFullVariableList() << std::endl;
    // scope->printVariableList();
    return ret;
  }
  std::string compileBinaryOperation(op_T binOp) {
    std::string ret;
    switch (binOp) {
    case OP_ADD:
      ret = "add rax, rbx\n";
      break;
    case OP_SUB:
      ret = "sub rax, rbx\n";
      break;
    case OP_MUL:
      ret = "mul rbx\n";
      break;
    case OP_DIV:
      ret = "div rbx\n";
      break;
    default: {
      std::cout << "Unimplemented Binary Operation: " << getOPType(binOp)
                << std::endl;
      exit(1);
    }
    }
    return ret;
  }
  std::string compileExpression(AST *ast, Scope *scope) {
    std::string ret;

    // std::cout << "MY EXPRESSION TYPE: " << getASTType(ast->type) <<
    // std::endl;
    std::cout << getASTType(ast->type) << std::endl;
    if (ast->type == AST_VARIABLE_RECALL) {
      ret += asmComment("Variable Recall in compileExpression");
      ret += "mov rax, QWORD [rsp+" +
             std::to_string(scope->getVariableOffset(ast->value)) + "]\n";
      ret += "sub rsp, 8\n";
      ret += "mov QWORD [rsp+8], rax\n";
    } else if (ast->type == AST_EXPRESSION_PRIMARY) {
      ret += asmComment("Primary Expession in compileExpression");
      ret += "sub rsp, 8\n";
      ret += "mov QWORD [rsp+8], " + ast->value + "\n";
    } else if (ast->type == AST_EXPRESSION) {
      ret += asmComment("Expession in compileExpression");
      ret += this->compileExpression(ast->left, scope);
      ret += this->compileExpression(ast->right, scope);
      ret += "mov rax, QWORD [rsp+8]\nadd rsp, 8\nmov rbx, QWORD[rsp + "
             "8]\nadd rsp, 8\n";
      ret += this->compileBinaryOperation(ast->binOp);
      ret += "sub rsp, 8\nmov QWORD [rsp+8], rax\n";
      // std::cout << ret << std::endl;
    } else {
      std::cout << "Unknown AST Type: " << getASTType(ast->type) << std::endl;
    }
    return ret;
  }
  std::string compileDeclaration(AST *ast, Scope *scope) {
    std::string ret;
    ret += "global " + ast->declarator->value + "\n";
    ret += ast->declarator->value + ":\n";
    if (ast->declarator->value != "main") {
      ret += asmComment("Standard function header");
      ret += "push rbp\nmov rbp, rsp\n";
    }
    ret += this->compileRootCompound(ast->declarator->body, scope) + "\n";
    if (ast->declarator->value == "main") {
      ret += asmComment("Default exit code");
      ret += "mov rax, 60\nmov rdi, 0\nsyscall\n";
    } else {
      ret += asmComment("Resetting stack");
      ret += "mov rsp, rbp\npop rbp\nret\n";
    }
    return ret;
  }
  std::string compileFunctionCall(AST *ast, Scope *scope) {
    std::string ret;
    for (auto &a : ast->arguments) {
      std::string pushed;
      if (a->type == AST_VARIABLE) {
        int offset = scope->getVariableOffset(a->value);
        pushed = "QWORD [rsp+" + std::to_string(offset) + "]";
      } else if (a->declType == "str") {
        pushed = this->addNewString(a->value);
      } else if (a->type == AST_EXPRESSION) {
        // std::cout << "HELP" << std::endl;
        if (a->body->type == AST_EXPRESSION_PRIMARY) {
          ret += asmComment("Parsing primary expression in function argument");
          ret += "mov QWORD [rsp+8], " + a->body->value + "\n";
          pushed = "QWORD [rsp+8]";
        }
      }
      ret += asmComment("Pushing argument to stack");
      ret += "push " + pushed + "\n";
    }
    ret += asmComment("Calling function");
    ret += "call " + ast->value + "\nadd rsp, 8";
    // + std::to_string(scope->variableListLength() * 8) + "\n";
    return ret;
  }
  std::string compileExtern(AST *ast, Scope *scope) {
    if (ast->externLang == "asm")
      return ast->value;
    return "";
  }
  std::string compileImport(AST *ast, Scope *scope) {
    std::string path = ast->value;
    path.erase(path.begin());
    path.pop_back();
    return oceanCompile(path, 0);
  }
  std::string generateDataSection() {
    std::string ret = "section .data\n";
    ret +=
        asmComment("Constants like strings and maybe const ints? not sure yet");
    for (auto &s : this->strings) {
      ret += s[0] + ": db " + s[1] + ", 0xa, 0\n";
    }
    return ret;
  }
};
