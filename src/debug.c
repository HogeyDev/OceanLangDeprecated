#include "include/debug.h"
#include "include/AST.h"
#include "include/token.h"
#include "include/types.h"
#include <stdio.h>

void print_debug_info(parser_T* parser) {
	printf("Error at %d:%d\n", parser->lexer->line, parser->lexer->col);
}

void print_error(char* message, parser_T* parser) {
	printf("%s\n", message);
	print_debug_info(parser);
	exit(1);
}

void print_ast_tree(AST_T* ast) {
	print_ast_tree_indent(ast, 0);
}

void print_ast_tree_indent(AST_T* ast, int indent) {
	printf("\n");	for (int i = 0; i < indent; i++) {
		printf("\t");
	}
	/*printf("%s\n", ast_type_to_str(ast->ast_type));*/
	if (ast->ast_type == AST_PROGRAM) {
		printf("{type=%s, children=[", ast_type_to_str(ast->ast_type));
		for (int i = 0; i < (int) ast->children->size; i++) {
			AST_T* child_ast = (AST_T*) ast->children->items[i];
			print_ast_tree_indent(child_ast, indent+1);
		}
		printf("]}");
	} else if (ast->ast_type == AST_FUNCTION_DECLARATION) {
		printf("{type=%s, datatype=%s, name=%s, parameters=[", ast_type_to_str(ast->ast_type), datatype_to_int(ast->datatype), ast->name);
		for (int i = 0; i < (int) ast->parameters->size; i++) {
			AST_T* child_ast = (AST_T*) ast->parameters->items[i];
			print_ast_tree_indent(child_ast, indent+1);
		}
		printf("], function_body=");
		print_ast_tree_indent(ast->function_body, indent+1);
		printf("}");
	} else if (ast->ast_type == AST_BLOCK) {
		printf("{type=%s, children=[", ast_type_to_str(ast->ast_type));
		for (int i = 0; i < (int) ast->children->size; i++) {
			AST_T* child_ast = (AST_T*) ast->children->items[i];
			print_ast_tree_indent(child_ast, indent+1);
		}
		printf("]}");
	} else if (ast->ast_type == AST_RETURN_STATEMENT) {
		printf("{type=%s, datatype=%s, int_value=%d}", ast_type_to_str(ast->ast_type), datatype_to_int(ast->datatype), ast->int_value);
	} else if (ast->ast_type == AST_FUNCTION_CALL) {
		printf("{type=%s, name=%s, arguments=[", ast_type_to_str(ast->ast_type), ast->name);
		for (int i = 0; i < (int) ast->arguments->size; i++) {
			AST_T* child_ast = (AST_T*) ast->arguments->items[i];
			print_ast_tree_indent(child_ast, indent+1);
		}
		printf("]}");
	} else if (ast->ast_type == AST_ARGUMENT) {
		printf("{type=%s, datatype=%s, str_value=%s}", ast_type_to_str(ast->ast_type), datatype_to_int(ast->datatype), ast->str_value);
	} else if (ast->ast_type == AST_EXTERN_STATEMENT) {
		printf("{type=%s, extern_language=%s, str_value=%s}", ast_type_to_str(ast->ast_type), ast->extern_language, ast->str_value);
	} else if (ast->ast_type == AST_IMPORT_STATEMENT) {
		printf("{type=%s, str_value=%s}", ast_type_to_str(ast->ast_type), ast->str_value);
	}
}

/*void print_ast_tree_loop(AST_T* ast) {*/
	/*unsigned int indent = 0;*/

	/*for (int i = 0; i < ast->children->size; i++) {*/
		/*AST_T* child_ast = (AST_T*) ast->children->items[i];*/
		/*indent++;*/
		/*ast_type_to_str(child_ast->ast_type);*/
		/*[>printf("{\n\ttype=%s\n", ast_type_to_str(child_ast->ast_type));<]*/
	/*}*/
/*}*/
