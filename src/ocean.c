#include "include/ocean.h"
#include "include/asm_frontend.h"
#include "include/io.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/preprocess.h"
#include "include/token.h"
#include "include/debug.h"
#include "include/visitor.h"
#include <stdlib.h>
#include <stdio.h>

void compile_ocean(char* src) {
	lexer_T* lexer = init_lexer(src);
	parser_T* parser = init_parser(lexer);
	AST_T* root = parser_parse(parser);
	preprocess_T* preprocessor = init_preprocess(root);
	root = preprocessor_preprocess(preprocessor);
	visitor_T* visitor = init_visitor(root);

	/*print_ast_tree(root);*/
	list_T* strings = init_list(sizeof(char*));
	visitor_visit(visitor, visitor->ast, visitor->stack_frame, strings);

	char* asm_code = asm_program(root, visitor->stack_frame, strings);

	write_file("main.asm", asm_code);
	/*sh("nasm -f elf32 main.asm");*/
	/*sh("ld -m elf_i386 -o a.out main.o");*/
	/*sh("rm main.asm main.o");*/

	/*token_T* tok = 0;*/

	/*while ((tok = lexer_next_token(lexer))->type != TOKEN_EOF) {*/
		/*printf("%s\n", token_to_str(tok));*/
	/*}*/
	/*printf("%s\n", token_to_str(tok));*/

	/*for (unsigned int i = 0; i < root->children->size; i++) {*/
		/*printf("{ NAME: `%s`, VALUE: `%s` }", (char*)root->children->items[i], "PLACEHOLDER");*/
	/*}*/
}

void compile_file(const char* filepath) {
	char* src = read_file(filepath);
	compile_ocean(src);

	free(src);
}
