#include "include/asm_frontend.h"
#include "include/AST.h"
#include "include/list.h"
#include "include/stack_frame.h"
#include "include/types.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* generate_data_section(list_T* strings) {
	char* data_section = calloc(strlen("section .data\n"), sizeof(char));
	strcat(data_section, "section .data\n");

	for (int i = 0; i < strings->size; i++) {
		char* string = (char*) strings->items[i];

		char* string_template = ".S%d db \"%s\", 0\n";
		char* s = calloc(strlen(string_template) + 1, sizeof(char));
		sprintf(s, string_template, i, string);


		data_section = realloc(data_section, (strlen(data_section) + strlen(s) + 1) * sizeof(char));
		strcat(data_section, s);
	}

	return  data_section;
}

char* get_string_label(char* string, list_T* strings) {
	char* label = ".S%d";
	for (int i = 0; i < strings->size; i++) {
		if (strcmp(get_string_contents(string), (char*) strings->items[i]) == 0) {
			char* s = calloc(strlen(label) + 1, sizeof(char));
			sprintf(s, label, i);

			return s;
		}
	}
	return 0;
}

/*char* asm_assemble(AST_T* ast, stack_frame_T* stack_frame) {*/
	/*switch (ast->ast_type) {*/
		/*case AST_PROGRAM: return asm_program();*/
	/*}*/
/*}*/

char* asm_program(AST_T* root, stack_frame_T* stack_frame, list_T* strings) {
	if (root->ast_type != AST_PROGRAM) {
		printf("[ASM ERROR]: Top Level is %s, expected AST_PROGRAM", ast_type_to_str(root->ast_type));
		exit(1);
	}
	
	unsigned int found_main = 0;
	AST_T* entrypoint = 0;
	for (int i = 0; i < root->children->size; i++) {
		AST_T* child = (AST_T*) root->children->items[i];
		if (!child->name) continue;
		if (strcmp(child->name, "main") == 0) {
			found_main = 1;
			entrypoint = child;
			break;
		}	
	}
	if (!found_main) {
		printf("[ASM ERROR]: No Entrypoint Found\n");
		exit(1);
	}

	char* data_section = generate_data_section(stack_frame->stack);
	char* entrypoint_asm = asm_function_declaration(entrypoint, stack_frame->stack);
	char* extern_asm = asm_extern(root);
	char* exit_asm =	"mov eax, 1\n"
				"mov ebx, 0\n"
				"int 0x80\n";

	/*list_T* strings = asm_strings(root);*/

	/*for (int i = 0; i < strings->size; i++) {*/
		/*printf("\n=======: %s\n=======\n", (char*) strings->items[i]);*/
	/*}*/
	/*printf("\n%d\n", (int) strings->size);*/

	char* asm_code = calloc(
				strlen(exit_asm) +
				strlen(extern_asm) +
				strlen("section .text\n") +
				strlen(entrypoint_asm) +
				strlen(data_section) + 1,
				sizeof(char)
			);
	strcat(asm_code, data_section);
	strcat(asm_code, "section .text\n");
	strcat(asm_code, extern_asm);
	strcat(asm_code, entrypoint_asm);
	strcat(asm_code, exit_asm);

	return asm_code;
}

list_T* asm_strings(AST_T* ast) {
	list_T* strings = init_list(sizeof(char*));
	if (ast->children) {
		for (int i = 0; i < ast->children->size; i++) {
			AST_T* child = (AST_T*) ast->children->items[i];

			list_push_list_items(strings, asm_strings(child));
			/*printf("\n%s\n", ast_type_to_str(child->type));	*/
			if (child->datatype != DATA_TYPE_STR) continue;

			list_push(strings, get_string_contents(child->str_value));

			/*printf("%s\n", child->str_value);*/
		}
	}

	if (ast->arguments) {
		for (int i = 0; i < ast->arguments->size; i++) {
			AST_T* child = (AST_T*) ast->arguments->items[i];

			list_push_list_items(strings, asm_strings(child));
			
			if (child->datatype != DATA_TYPE_STR) continue;

			list_push(strings, get_string_contents(child->str_value));

			/*printf("%s\n", child->str_value);*/
		}
	}
	return strings;
}

char* asm_function_declaration(AST_T* ast, list_T* strings) {
	char* function_declaration = calloc(1, sizeof(char));

	char* function_declaration_template =	"global %s\n"
						"%s:\n"
						"push ebp\n"
						"mov ebp, esp\n";

	char* s = calloc(strlen(function_declaration_template) + 1, sizeof(char));
	if (strcmp(ast->name, "main") == 0) {
		sprintf(s, function_declaration_template, "_start", "_start");
	} else {
		sprintf(s, function_declaration_template, ast->name, ast->name);
	}

	function_declaration = realloc(function_declaration, (strlen(function_declaration) + strlen(s) + 1) * sizeof(char));
	strcat(function_declaration, s);

	for (int i = 0; i < ast->function_body->children->size; i++) {
		AST_T* child = (AST_T*) ast->function_body->children->items[i];
		switch (child->ast_type) {
			case AST_FUNCTION_CALL: {
				char* function_call = asm_function_call(child, strings);
				function_declaration = realloc(function_declaration, (strlen(function_declaration) + strlen(function_call) + 1) * sizeof(char));
				strcat(function_declaration, function_call);
				break;
			}
			case AST_RETURN_STATEMENT: {
				/*printf("RETURN STATEMENT\n");*/
				break;
			}
			default: {
				printf("Bad Type: %s\n", ast_type_to_str(child->ast_type));
				exit(1);
				break;
			}
		}
	}

	/*printf("\n%s", function_declaration);*/
	return function_declaration;
}

char* asm_function_call(AST_T* ast, list_T* strings) {
	char* function_call = calloc(1, sizeof(char));

	char* argument_template = "push %s\n";
	int num_arguments = ast->arguments->size;
	for (int i = 0; i < ast->arguments->size; i++) {
		AST_T* arg = (AST_T*) ast->arguments->items[i];

		char* s = calloc(strlen(argument_template) + 1, sizeof(char));
		if (arg->datatype == DATA_TYPE_STR) {
			sprintf(s, argument_template, get_string_label(arg->str_value, strings));
		} else {
			sprintf(s, argument_template, arg->str_value);
		}
		function_call = realloc(function_call, (strlen(function_call) + strlen(s) + 1) * sizeof(char));
		strcat(function_call, s);
	}

	char* call_template =	"call %s\n"
				"add esp, %d\n";
	char* s = calloc(strlen(call_template) + 1, sizeof(char));
	sprintf(s, call_template, ast->name, num_arguments * 4);

	function_call = realloc(function_call, (strlen(function_call) + strlen(s)) * sizeof(char));
	strcat(function_call, s);

	/*printf("%s", function_call);*/

	return function_call;
}

char* asm_extern(AST_T* ast) {
	char* extern_code = calloc(1, sizeof(char));
	for (int i = 0; i < ast->children->size; i++) {
		AST_T* child = (AST_T*) ast->children->items[i];

		if (child->ast_type != AST_EXTERN_STATEMENT) continue;

		char* code = get_string_contents(child->str_value);
		extern_code = realloc(extern_code, (strlen(extern_code) + strlen(code) + 1) * sizeof(char));
		strcat(extern_code, code);

	}
	return extern_code;
}
