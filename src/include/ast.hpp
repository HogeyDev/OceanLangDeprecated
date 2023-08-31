#pragma once
#include <string>
#include <vector>

typedef enum {
    AST_ROOT,
    AST_IMPORT,
    AST_EXTERN,
    AST_WHILE,
    AST_FOR,
    AST_IF,
    AST_FUNCTION_DECLARATION,
    AST_FUNCTION_CALL,
    AST_ARGUMENT,
    AST_COMPOUND,
    AST_NULL,
    AST_TYPE,
    AST_DECLARATION,
    AST_PARAMETER,
    AST_EXPRESSION,
    AST_VARIABLE_ASSIGNMENT,
    AST_VARIABLE,
} ast_T;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_DIV,
    OP_MUL,
    OP_LT, // less than
    OP_GT, // greator than
    OP_ASSIGN,
    OP_CMP, // comparison
    OP_LTET, // less than equal to
    OP_GTET, // greator than equal to
} op_T;

class AST {
    public:
        ast_T type;
        std::string value;
        std::string externLang;
        std::string declType;
        AST* declarator;
        AST* body;
        AST* left;
        AST* right;
        op_T binOp;
        std::vector<AST*> arguments;
        std::vector<AST*> children;

        AST(ast_T type) {
            this->type = type;
        }
};

void printAST(AST* ast, std::string tabs, int finalReturn);
void printAST(AST* ast);
std::string getASTType(ast_T type);
