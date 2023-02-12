#pragma once

#include "AST.h"
#include "stack_frame.h"

typedef struct VISITOR_STRUCT {
	AST_T* ast;
	stack_frame_T* stack_frame;
} visitor_T;

visitor_T* init_visitor(AST_T* ast);

AST_T* visitor_visit(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);

AST_T* visitor_visit_extern(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);

AST_T* visitor_visit_function_declaration(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);

AST_T* visitor_visit_argument(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);

AST_T* visitor_visit_program(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);

AST_T* visitor_visit_return(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);

AST_T* visitor_visit_assignment(visitor_T* visitor, AST_T* ast, stack_frame_T* stack_frame, list_T* strings);
