#include "include/preprocess.h"
#include "include/AST.h"
#include "include/debug.h"
#include "include/lexer.h"
#include "include/list.h"
#include "include/parser.h"
#include "include/io.h"
#include "include/utils.h"
#include  <unistd.h>

preprocess_T* init_preprocess(AST_T* ast) {
	preprocess_T* preprocessor = calloc(1, sizeof(struct preprocess_T*));

	preprocessor->ast = ast;

	return preprocessor;
}

AST_T* preprocessor_preprocess(preprocess_T* preprocessor) {
	AST_T* ast = preprocessor->ast;

	/*print_ast_tree(ast);*/
	/*printf("%d\n", (int) ast->children->size);*/
	while (ast_contains_ast_type(ast, AST_IMPORT_STATEMENT)) {
		for (int i = 0; i < ast->children->size; i++) {
			AST_T* child = (AST_T*) ast->children->items[i];
			if (child->ast_type != AST_IMPORT_STATEMENT) continue;

			char* file_contents = read_file(get_string_contents(child->str_value));
			/*char* file_contents = read_file("examples/io.wv");*/
			/*printf("%s\n", file_contents);*/

			lexer_T* child_lexer = init_lexer(file_contents);
			parser_T* child_parser = init_parser(child_lexer);
			AST_T* file_parsed = parser_parse(child_parser);
			/*preprocess_T* child_preprocess = init_preprocess(file_parsed);*/
			/*AST_T* processed = preprocessor_preprocess(child_preprocess);*/
			/*print_ast_tree(file_parsed);*/

			ast->children = list_remove(ast->children, i);
			/*print_ast_tree(ast);*/
			ast->children = list_insert_list_items(ast->children, file_parsed->children, i);
			/*print_ast_tree(ast);*/

			/*printf("%d\n", (int) ast->children->size);*/
			break;
		}
	}
	/*print_ast_tree(ast);*/
	/*printf("%d\n", (int) ast->children->size);*/

	return ast;
}
