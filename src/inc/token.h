#pragma once

typedef struct TOKEN_STRUCT {
	char* value;
	enum {
		TOKEN_ID,
		TOKEN_LPAREN,
		TOKEN_RPAREN,
		TOKEN_COMMA,
		TOKEN_STAR,
		TOKEN_FSLASH,
		TOKEN_BSLASH,
		TOKEN_PLUS,
		TOKEN_MINUS,
		TOKEN_LBRACKET,
		TOKEN_RBRACKET,
		TOKEN_LBRACE,
		TOKEN_RBRACE,
		TOKEN_LT,
		TOKEN_GT,
		TOKEN_EQUALS,
		TOKEN_SEMI,
		TOKEN_NUM,
		TOKEN_STR,
		TOKEN_COLON,
		TOKEN_AMPERSAND,
		TOKEN_PIPE,
		TOKEN_EOF,
		TOKEN_STATEMENT,
		TOKEN_DOUBLE_QUOTE,
		TOKEN_MEMBER_ACCESSOR,
	} type;
} token_T;

token_T* init_token(char* value, int type);

const char* token_type_to_str(int type);

char* token_to_str(token_T* token);
