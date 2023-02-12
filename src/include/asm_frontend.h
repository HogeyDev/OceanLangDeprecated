#pragma once

#include "AST.h"
#include "stack_frame.h"

char* generate_data_section(list_T* strings);

char* get_string_label(char* string, list_T* strings);

//char* asm_assemble(AST_T* ast, stack_frame_T* stack_frame);

char* asm_program(AST_T* root, stack_frame_T* stack_frame, list_T* strings);

char* asm_function_declaration(AST_T* ast, list_T* strings);

list_T* asm_strings(AST_T* ast);

char* asm_function_call(AST_T* ast, list_T* strings);

char* asm_extern(AST_T* ast);
