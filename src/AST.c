#include "include/AST.h"
#include "include/types.h"
#include <stdlib.h>

AST_T* init_ast(int type) {
	AST_T* ast = calloc(1, sizeof(struct AST_STRUCT));
	ast->ast_type = type;

	/*if (type == AST_COMPOUND)*/

	/*ast->stack_frame = (void*) 0;*/
	/*ast->multiplier = 1;*/
	ast->datatype = DATA_TYPE_UNKNOWN;
	ast->array_child_datatype = DATA_TYPE_UNKNOWN;
	ast->children = init_list(sizeof(struct AST_STRUCT*));
	ast->member_child = init_list(sizeof(char*));

	return ast;
}

const char* ast_type_to_str(int type) {
	switch (type) {
		case AST_PROGRAM: return "AST_PROGRAM";
		case AST_IMPORT_STATEMENT: return "AST_IMPORT_STATEMENT";
		case AST_FUNCTION_DECLARATION: return "AST_FUNCTION_DECLARATION";
		case AST_PARAMETER: return "AST_PARAMETER";
		case AST_BLOCK: return "AST_BLOCK";
		case AST_FUNCTION_CALL: return "AST_FUNCTION_CALL";
		case AST_ARGUMENT: return "AST_ARGUMENT";
		case AST_RETURN_STATEMENT: return "AST_RETURN_STATEMENT";
		case AST_EXTERN_STATEMENT: return "AST_EXTERN_STATEMENT";
		case AST_ASSIGNMENT: return "AST_ASSIGNMENT";
		default: return "not_stringable";
	}
}

unsigned int ast_contains_ast_type(AST_T* ast, int type) {
	for (int i = 0; i < ast->children->size; i++) {
		AST_T* child = (AST_T*) ast->children->items[i];
		if (child->ast_type == type) return 1;
	}
	return 0;
}
