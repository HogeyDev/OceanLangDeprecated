#pragma once

#include "list.h"
#include "stack_frame.h"
#include "types.h"

struct VISITOR_STRUCT;

typedef struct AST_STRUCT {
	enum {
		//AST_COMPOUND,
		//AST_FUNCTION,
		//AST_CALL,
		//AST_ASSIGNMENT = 99,
		//AST_DEFINITION_TYPE,
		//AST_VARIABLE,
		//AST_STATEMENT_RETURN,
		//AST_STATEMENT_IF,
		//AST_ACCESS,
		//AST_NUM,
		//AST_STRING,
		//AST_BINOP,
		//AST_NOOP,
		//AST_MEMBER_ACCESS,
		//AST_ARGUMENT,
		//AST_POINTER,
		//AST_PROGRAM,
		//AST_FUNCTION_DECLARATION,
		
		AST_PROGRAM,
		AST_IMPORT_STATEMENT,
		AST_FUNCTION_DECLARATION,
		AST_PARAMETER,
		AST_BLOCK,
		AST_FUNCTION_CALL,
		AST_ARGUMENT,
		AST_RETURN_STATEMENT,
		AST_MEMBER_ACCESS,
		AST_EXTERN_STATEMENT,
		AST_ASSIGNMENT,
		AST_VARIABLE,
	} type;

	//data_type dtype;

	//list_T* children;
	//char* name;
	//char* string_value;
	//struct AST_STRUCT* value;
	//struct AST_STRUCT* left;
	//struct AST_STRUCT* right;
	//int op;
	//int int_value;
	//int id;
	//int stack_index;
	//int multiplier;
	//int pointer_depth;
	//struct AST_STRUCT* (*fptr)(struct VISITOR_STRUCT* visitor, struct AST_STRUCT* node, list_T* list);
	//stack_frame_T* stack_frame;
	
	data_type datatype;
	data_type array_child_datatype;
	list_T* children;
	list_T* member_child;
	list_T* parameters;
	list_T* arguments;
	char* name;
	char* str_value;
	char* root;
	char* extern_language;
	int ast_type;
	int int_value;
	struct AST_STRUCT* function_body;
} AST_T;

AST_T* init_ast(int type);

const char* ast_type_to_str(int type);

unsigned int ast_contains_ast_type(AST_T* ast, int type);
