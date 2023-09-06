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
  std::string addNewString(std::string name) {
    std::vector<std::string> row;
    for (auto &s : this->strings) {
      if (s[1] != name)
        continue;
      return s[0];
    }
    row.push_back("STR" + std::to_string(this->strings.size()));
    row.push_back(name);
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
      scope->addNewVariable(ast->declType, ast->declarator->value, pushed);
    } else if (ast->declType == "int") {
      ret += "sub rsp, 8\n";
      ret += "mov QWORD [rsp+8], " + ast->declarator->value + "\n";
      std::cout << "NAME: " << ast->declarator->value
                << " VALUE: " << ast->value << std::endl;
      scope->addNewVariable(ast->declType, ast->declarator->value, ast->value);
    }
    return ret;
  }
  std::string compileDeclaration(AST *ast, Scope *scope) {
    std::string ret;
    ret += "global " + ast->declarator->value + "\n";
    ret += ast->declarator->value + ":\n";
    if (ast->declarator->value != "main")
      ret += "push rbp\nmov rbp, rsp\n";
    ret += this->compileRootCompound(ast->declarator->body, scope) + "\n";
    if (ast->declarator->value == "main") {
      ret += "mov rax, 60\nmov rdi, 0\nsyscall\n";
    } else {
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
          ret += "mov QWORD [rsp+8], " + a->body->value + "\n";
          pushed = "QWORD [rsp+8]";
        }
      }
      ret += "push " + pushed + "\n";
    }
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
    for (auto &s : this->strings) {
      ret += s[0] + ": db " + s[1] + ", 0xa, 0\n";
    }
    return ret;
  }
};
