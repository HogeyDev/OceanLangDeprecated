#include "include/visitor.h"
#include "include/AST.h"
#include "include/stack_frame.h"
#include "include/types.h"
#include "include/utils.h"

visitor_T* init_visitor(AST_T* ast) {
	visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
	visitor->ast = ast;
	visitor->stack_frame = init_stack_frame();

	return visitor;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	switch (ast->ast_type) {
		case AST_EXTERN_STATEMENT: return visitor_visit_extern(visitor, ast, stack_frame, strings);
		case AST_FUNCTION_DECLARATION: return visitor_visit_function_declaration(visitor, ast, stack_frame, strings);
		case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor, ast, stack_frame, strings);
		case AST_ARGUMENT: return visitor_visit_argument(visitor, ast, stack_frame, strings);
		case AST_PROGRAM: return visitor_visit_program(visitor, ast, stack_frame, strings);
		case AST_RETURN_STATEMENT: return visitor_visit_return(visitor, ast, stack_frame, strings);
		case AST_ASSIGNMENT: return visitor_visit_assignment(visitor, ast, stack_frame, strings);
		default: {
				printf("Unknown AST type: %s\n", ast_type_to_str(ast->ast_type));
				exit(1);
			 }
	}
}

AST_T* visitor_visit_extern(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	return ast;
}

AST_T* visitor_visit_function_declaration(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	for (int i = 0; i < (int) ast->function_body->children->size; i++) {
		AST_T* child = (AST_T*) ast->function_body->children->items[i];
		visitor_visit(visitor, child, stack_frame, strings);
	}
	return ast;
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	for (int i = 0; i < (int) ast->arguments->size; i++) {
		AST_T* argument = (AST_T*) ast->arguments->items[i];
		visitor_visit(visitor, argument, stack_frame, strings);
	}
	return ast;
}

AST_T* visitor_visit_argument(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	if (ast->datatype == DATA_TYPE_STR) {
		list_push(stack_frame->stack, get_string_contents(ast->str_value));
	}
	return ast;
}

AST_T* visitor_visit_program(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	for (int i = 0; i < (int) ast->children->size; i++) {
		AST_T* child = (AST_T*) ast->children->items[i];
		visitor_visit(visitor, child, stack_frame, strings);
	}
	return ast;
}

AST_T* visitor_visit_return(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	if (ast->datatype == DATA_TYPE_STR) {
		list_push(stack_frame->stack, get_string_contents(ast->str_value));
	}

	return ast;
}

AST_T* visitor_visit_assignment(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings) {
	
}
