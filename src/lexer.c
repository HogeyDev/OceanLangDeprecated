#include "include/lexer.h"
#include "include/macros.h"
#include "include/utils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

lexer_T* init_lexer(char* src) {
	lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
	lexer->src = src;
	lexer->i = 0;
	lexer->c = lexer->src[lexer->i];
	lexer->src_size = strlen(lexer->src);
	lexer->line = 1;
	lexer->col = 1;

	return lexer;
}

void lexer_advance(lexer_T* lexer) {
	if (lexer->i < lexer->src_size && lexer->c != '\0') {
		if (lexer->c == '\n') {
			lexer->line++;
			lexer->col = 0;
		} else { lexer->col++; }
		lexer->i++;
		lexer->c = lexer->src[lexer->i];
	}
}

char lexer_peek(lexer_T* lexer, int offset) {
	return lexer->src[MIN(lexer->i + offset, lexer->src_size)];
}

token_T* lexer_advance_with(lexer_T* lexer, token_T* token) {
	lexer_advance(lexer);
	
	return token;
}

token_T* lexer_advance_current(lexer_T* lexer, int type) {
	char* value = calloc(2, sizeof(char));
	value[0] = lexer->c;
	value[1] = '\0';
	
	token_T* token = init_token(value, type);
	lexer_advance(lexer);

	return token;
}

void lexer_skip_whitespace(lexer_T* lexer) {
	while (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t') {
		lexer_advance(lexer);
	}
}

token_T* lexer_parse_id(lexer_T* lexer) {
	int token_type = TOKEN_ID;

	char* value = calloc(1, sizeof(char));

	while(isalpha(lexer->c)) {
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){lexer->c, 0});
		lexer_advance(lexer);
	}

	if (
		strcmp(value, "return") == 0 ||
		strcmp(value, "if") == 0 ||
		strcmp(value, "import") == 0 ||
		strcmp(value, "extern") == 0
	) {
		token_type = TOKEN_STATEMENT;
	}

	lexer->i--;
	lexer->c = lexer->src[lexer->i];

	return init_token(value, token_type);
}

token_T* lexer_parse_number(lexer_T* lexer) {
	char* value = calloc(1, sizeof(char));
	while(isdigit(lexer->c)) {
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){lexer->c, 0});
		lexer_advance(lexer);
	}
	lexer->i--;
	lexer->c = lexer->src[lexer->i];

	return init_token(value, TOKEN_NUM);
}

token_T* lexer_parse_string(lexer_T* lexer) {
	char* value = calloc(2, sizeof(char));
	strcat(value, "\"");
	do {
		lexer_advance(lexer);
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){lexer->c, 0});
	} while (lexer->c != '"');

	return init_token(value, TOKEN_STR);
}

token_T* lexer_next_token(lexer_T* lexer) {
	while (lexer->c != '\0') {
		lexer_skip_whitespace(lexer);
		if (isalpha(lexer->c))
			return lexer_advance_with(lexer, lexer_parse_id(lexer));

		if (isdigit(lexer->c))
			return lexer_advance_with(lexer, lexer_parse_number(lexer));

		if (lexer->c == '"')
			return lexer_advance_with(lexer, lexer_parse_string(lexer));

		switch (lexer->c) {
			case '=': return lexer_advance_current(lexer, TOKEN_EQUALS);
			case '(': return lexer_advance_current(lexer, TOKEN_LPAREN);
			case ')': return lexer_advance_current(lexer, TOKEN_RPAREN);
			case '{': return lexer_advance_current(lexer, TOKEN_LBRACE);
			case '}': return lexer_advance_current(lexer, TOKEN_RBRACE);
			case ':': {
				if (lexer_peek(lexer, 1) == ':') return lexer_advance_with(lexer, lexer_advance_with(lexer, init_token("::", TOKEN_MEMBER_ACCESSOR)));
				else return lexer_advance_current(lexer, TOKEN_COLON);
			}
			case ',': return lexer_advance_current(lexer, TOKEN_COMMA);
			case '<': return lexer_advance_current(lexer, TOKEN_LT);
			case '>': return lexer_advance_current(lexer, TOKEN_GT);
			case ';': return lexer_advance_current(lexer, TOKEN_SEMI);
			case '*': return lexer_advance_current(lexer, TOKEN_STAR);
			case '[': return lexer_advance_current(lexer, TOKEN_LBRACKET);
			case ']': return lexer_advance_current(lexer, TOKEN_RBRACKET);
			case '|': return lexer_advance_current(lexer, TOKEN_PIPE);
			case '&': return lexer_advance_current(lexer, TOKEN_AMPERSAND);
			case '/': return lexer_advance_current(lexer, TOKEN_FSLASH);
			case '\\': return lexer_advance_current(lexer, TOKEN_BSLASH);
			case '+': return lexer_advance_current(lexer, TOKEN_PLUS);
			case '-': return lexer_advance_current(lexer, TOKEN_MINUS);
			case '"': return lexer_advance_current(lexer, TOKEN_DOUBLE_QUOTE);
			
			case '\0': break;
			default: printf("[LEXER]: Unexpected character: %c\n", lexer->c);
		}
	}

	return init_token(0, TOKEN_EOF);
}
