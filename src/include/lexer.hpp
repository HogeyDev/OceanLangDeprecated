#pragma once
#include <cctype>
#include <iostream>
#include <string>
#include "token.hpp"
#include "ast.hpp"
#include "util.hpp"

class Lexer {
    public:
        std::string input;
        int pos = 0;
        int line = 0;
        int column = 0;
        char c = 0; 

        Lexer(std::string code) {
            this->input = code;
            this->c = this->input[this->pos];
        }

        void printError() {
            std::cout << "[LEXER] Unrecognized Character: " << this->c << " (" << this->line << ":" << this->column << ")" << std::endl;
        }

        void advance() {
            if (this->c != '\0') {
                if (this->c == '\n') {
                    this->line++;
                    this->column = 0;
                } else {
                    this->column++;
                }
                this->pos++;
                this->c = this->input[this->pos];
            }
        }
        Token* advanceWith(Token* t) {
            this->advance();
            return t;
        }
        Token* advanceWith(token_T t) {
            this->advance();
            return new Token(t);
        }

        Token* nextToken() {
            this->skipWhitespace();
            this->skipComments();

            if (isalpha(this->c)) {
                return this->parseId();
            }
            if (isdigit(this->c)) {
                std::cout << this->c << std::endl;
                return this->parseInt();
            }

            switch (this->c) {
                case '"': return this->parseString();
                case '(': return this->advanceWith(TOKEN_LPAREN);
                case ')': return this->advanceWith(TOKEN_RPAREN);
                case '{': return this->advanceWith(TOKEN_LBRACE);
                case '}': return this->advanceWith(TOKEN_RBRACE);
                case ';': return this->advanceWith(TOKEN_SEMICOLON);
                case '=': return this->advanceWith(TOKEN_EQUALS);
                case '/': this->skipComments(); return this->nextToken();
                case '\0': return new Token(TOKEN_EOF);
                default: {
                             this->printError();
                             exit(0);
                         }
            }
        }
        char peek(int n) {
            if ((size_t)(this->pos + n) > this->input.size())
                return '\0';
            else return this->input[this->pos + n];
        }
        void skipWhitespace() {
            while (isspace(this->c)) {
                this->advance();
            }
        }
        void skipComments() {
            if (this->c == '/' && this->peek(1) == '/') {
                while(this->c != '\n') {
                    this->advance();
                }
                this->skipWhitespace();
            }
        }
        Token* parseId() {
            Token* token = new Token(TOKEN_ID);
            while (isalpha(this->c)) {
                token->value += this->c;
                this->advance();
            }
            if (isType(token->value)) token->type = TOKEN_PRIM_TYPE;
            if (token->value == "while") token->type = TOKEN_STATEMENT;
            if (token->value == "for") token->type = TOKEN_STATEMENT;
            if (token->value == "if") token->type = TOKEN_STATEMENT;
            return token;
        }
        Token* parseString() {
            Token* token = new Token(TOKEN_STRING);
            token->strValue += this->c;
            do {
                this->advance();
                token->strValue += this->c;
            } while (this->c != '"' && this->c != '\0');
            this->advance();
            return token;
        }
        Token* parseInt() {
            Token* token = new Token(TOKEN_INT);
            do {
                token->intValue = token->intValue * 10 + (this->c - '0');
                this->advance();
            } while (isdigit(this->c));
            return token;
        }
};
