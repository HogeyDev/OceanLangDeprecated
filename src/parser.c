#include "include/parser.h"
#include "include/AST.h"
#include "include/debug.h"
#include "include/list.h"
#include "include/token.h"
#include "include/types.h"
#include <stdio.h>

parser_T* init_parser(lexer_T* lexer) {
	parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
	parser->lexer = lexer;
	parser->token = lexer_next_token(lexer);
	parser->last_token = 0;
	parser->old_tokens = init_list(sizeof(struct TOKEN_STRUCT));
	return parser;
}

token_T* parser_eat(parser_T* parser, int type) {
	if (parser->last_token)
		parser->old_tokens = list_insert(parser->old_tokens, parser->last_token, 0);
	parser->last_token = parser->token;
	if (parser->token->type != type) {
		printf("[PARSER]: Unexpected Token: `%s`, was expecting: `%s`\n", token_to_str(parser->token), token_type_to_str(type));
		print_debug_info(parser);
		exit(1);
	}

	parser->token = lexer_next_token(parser->lexer);
	return parser->token;
}

AST_T* parser_parse(parser_T* parser) {
	return parser_parse_program(parser);
}

AST_T* parser_parse_program(parser_T* parser) {
	AST_T* ast = init_ast(AST_PROGRAM);

	while (parser->token->type != TOKEN_EOF) {
		switch (parser->token->type) {
			case TOKEN_ID: list_push(ast->children, parser_parse_id(parser)); break;
			case TOKEN_STATEMENT: list_push(ast->children, parser_parse_statement(parser)); break;
			default: {
					printf("Unknown Token: `%s`\n", token_to_str(parser->token));
					print_debug_info(parser);
					exit(1);
				 }
		}
	}

	return ast;
}

AST_T* parser_parse_id(parser_T* parser) {
	parser_eat(parser, TOKEN_ID);
	switch (parser->token->type) {
		case TOKEN_LPAREN: return parser_parse_function(parser);
		case TOKEN_MEMBER_ACCESSOR: return parser_parse_member_access(parser);
		case TOKEN_ID: return parser_parse_id(parser);
		case TOKEN_EQUALS: return parser_parse_assignment(parser);
		default: {
				printf("Unknown ID: %s\n", token_type_to_str(parser->token->type));
				print_debug_info(parser);
				exit(1);
			 }
	}
	/*if (parser->token->type == TOKEN_LPAREN) {*/
		/*ast = parser_parse_function(parser);*/
	/*} else if (parser->token->type) {*/
		
	/*}*/
}

AST_T* parser_parse_function(parser_T* parser) {
	AST_T* ast = init_ast(AST_FUNCTION_CALL);
	
	ast->name = parser->last_token->value;

	AST_T* list = parser_parse_list(parser);

	if (parser->token->type == TOKEN_LBRACE) ast->ast_type = AST_FUNCTION_DECLARATION;
	if (ast->ast_type == AST_FUNCTION_DECLARATION) {
		ast->parameters = list->children;
		ast->function_body = parser_parse_block(parser);
	} else {
		ast->arguments = list->children;
	}

	return ast;
}

AST_T* parser_parse_block(parser_T* parser) {
	AST_T* ast = init_ast(AST_BLOCK);

	parser_eat(parser, TOKEN_LBRACE);

	while (parser->token->type != TOKEN_RBRACE && parser->token->type != TOKEN_EOF) {
		switch (parser->token->type) {
			case TOKEN_ID: list_push(ast->children, parser_parse_id(parser)); break;
			case TOKEN_STATEMENT: list_push(ast->children, parser_parse_statement(parser)); break;
			case TOKEN_SEMI: parser_eat(parser, TOKEN_SEMI); break;
			default: {
					printf("Unknown Token while parsing block: `%s`\n", token_to_str(parser->token));
					print_debug_info(parser);
					exit(1);
				 }
		}
	}

	parser_eat(parser, TOKEN_RBRACE);

	return ast;
}

AST_T* parser_parse_list(parser_T* parser) {
	AST_T* ast = init_ast(AST_FUNCTION_CALL);

	parser_eat(parser, TOKEN_LPAREN);

	while (parser->token->type != TOKEN_RPAREN && parser->token->type != TOKEN_EOF) {
		printf("%s\n", token_to_str(parser->token));
		/*print_debug_info(parser);*/
		/*exit(1);*/
		AST_T* child = init_ast(AST_ARGUMENT);
		if (typename_to_int(parser->token->value) != DATA_TYPE_UNKNOWN) {
			child->datatype = typename_to_int(parser->token->value);
			parser_eat(parser, TOKEN_ID);
			child->name = parser->token->value;
			parser_eat(parser, TOKEN_ID);
		} else {
			/*it is a function call*/
			if (parser->token->type == TOKEN_STR) {
				child->datatype = DATA_TYPE_STR;
				child->str_value = parser->token->value;
				parser_eat(parser, TOKEN_STR);
			} else if (parser->token->type == TOKEN_NUM) {
				/*we only have one type of number (integers)*/
				child->datatype = DATA_TYPE_INT;
				child->int_value = atoi(parser->token->value);
				parser_eat(parser, TOKEN_NUM);
			} else {
				child->ast_type = AST_VARIABLE;
				child->name = parser->token->value;
				parser_eat(parser, TOKEN_ID);
			}
		}
		if (parser->token->type == TOKEN_COMMA) parser_eat(parser, TOKEN_COMMA);
		list_push(ast->children, child);

	}

	parser_eat(parser, TOKEN_RPAREN);

	return ast;
}

AST_T* parser_parse_statement(parser_T* parser) {
	if (strcmp(parser->token->value, "import") == 0) return parser_parse_import(parser);
	if (strcmp(parser->token->value, "return") == 0) return parser_parse_return(parser);
	if (strcmp(parser->token->value, "extern") == 0) return parser_parse_extern(parser);
	return 0;
}

AST_T* parser_parse_import(parser_T* parser) {
	AST_T* ast = init_ast(AST_IMPORT_STATEMENT);

	parser_eat(parser, TOKEN_STATEMENT);
	ast->str_value = parser->token->value;
	parser_eat(parser, TOKEN_STR);
	
	/*printf("<ast_type=`%s`, type=`%d`, str_value=`%s`>\n", ast_type_to_str(ast->ast_type), ast->ast_type, ast->str_value);*/

	return ast;
}

AST_T* parser_parse_return(parser_T* parser) {
	AST_T* ast = init_ast(AST_RETURN_STATEMENT);

	parser_eat(parser, TOKEN_STATEMENT);

	if (parser->token->type == TOKEN_NUM) {
		ast->datatype = DATA_TYPE_INT;
		ast->int_value = atoi(parser->token->value);
		parser_eat(parser, TOKEN_NUM);
	} else if (parser->token->type == TOKEN_STR) {
		ast->datatype = DATA_TYPE_STR;
		ast->str_value = parser->token->value;
		parser_eat(parser, TOKEN_STR);
	}

	return ast;
}

AST_T* parser_parse_member_access(parser_T* parser) {
	AST_T* ast = init_ast(AST_MEMBER_ACCESS);
	ast->root = parser->token->value;
	while (parser->token->type == TOKEN_MEMBER_ACCESSOR) {
		ast = init_ast(AST_MEMBER_ACCESS);
		parser_eat(parser, TOKEN_MEMBER_ACCESSOR);
	}
	return ast;
}

AST_T* parser_parse_extern(parser_T* parser) {
	AST_T* ast = init_ast(AST_EXTERN_STATEMENT);

	parser_eat(parser, TOKEN_STATEMENT);
	ast->extern_language = parser->token->value;
	if (strcmp(parser->token->value, "asm") == 0) {
		parser_eat(parser, TOKEN_ID);
		parser_eat(parser, TOKEN_EQUALS);
		ast->str_value = parser->token->value;
		parser_eat(parser, TOKEN_STR);
		parser_eat(parser, TOKEN_SEMI);
	} else {
		printf("Unknown External Language: %s\n", parser->token->value);
		exit(1);
	}

	return ast;
}

AST_T* parser_parse_assignment(parser_T* parser) {
	AST_T* ast = init_ast(AST_ASSIGNMENT);

	ast->name = parser->last_token->value;
	if (strcmp(((token_T*) parser->old_tokens->items[0])->value, "str") == 0) {
		printf("it's a string!\n");
		parser_eat(parser, TOKEN_EQUALS);
		parser_eat(parser, TOKEN_STR);
	}

	return ast;
}
