#pragma once
#include "parser.h"

void print_debug_info(parser_T* parser);

void print_error(char* message, parser_T* parser);

void print_ast_tree(AST_T* ast);

void print_ast_tree_indent(AST_T* ast, int indent);

//void print_ast_tree_loop(AST_T* ast);
