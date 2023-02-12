#pragma once

#include "lexer.h"
#include "AST.h"
#include "list.h"

typedef struct PARSER_STRUCT {
	lexer_T* lexer;
	token_T* token;
	token_T* last_token;
	list_T* old_tokens;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

token_T* parser_eat(parser_T* parser, int type);

AST_T* parser_parse(parser_T* parser);

AST_T* parser_parse_program(parser_T* parser);

AST_T* parser_parse_id(parser_T* parser);

AST_T* parser_parse_function(parser_T* parser);

AST_T* parser_parse_block(parser_T* parser);

AST_T* parser_parse_list(parser_T* parser);

AST_T* parser_parse_statement(parser_T* parser);

AST_T* parser_parse_import(parser_T* parser);

AST_T* parser_parse_return(parser_T* parser);

AST_T* parser_parse_member_access(parser_T* parser);

AST_T* parser_parse_extern(parser_T* parser);

AST_T* parser_parse_assignment(parser_T* parser);
