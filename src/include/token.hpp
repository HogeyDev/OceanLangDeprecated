#pragma once
#include <string>

typedef enum {
    TOKEN_ID,
    TOKEN_EOF,
    TOKEN_STRING,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_INT,
    TOKEN_NULL,
    TOKEN_PRIM_TYPE,
    TOKEN_STATEMENT,
    TOKEN_COMMA,
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
} token_T;

class Token {
    public:
        token_T type = TOKEN_NULL;
        std::string strValue = "";
        std::string value = "";
        int intValue = 0;

        Token(token_T type) {
            this->type = type;
        }
        Token(token_T type, std::string value) {
            this->type = type;
            if (type == TOKEN_STRING) {
                this->strValue = value;
            } else {
                this->value = value;
            }
        }
        Token(token_T type, int value) {
            this->type = type;
            this->intValue = value;
        }
};

void printToken(Token* token);
std::string getTokenType(token_T type);
