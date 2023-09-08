#include "token.hpp"
#include <iostream>
#include <string>
#include <vector>

void printToken(Token *token) {
  std::cout << "TOKEN(TYPE: '" << getTokenType(token->type) << "', VALUE: '"
            << token->value << "', INTVALUE: '" << token->intValue
            << "', STRVALUE: '" << token->strValue << "')" << std::endl;
}

std::string getTokenType(token_T type) {
  const static std::string names[] = {
      "TOKEN_ID",     "TOKEN_EOF",    "TOKEN_STRING",    "TOKEN_LPAREN",
      "TOKEN_RPAREN", "TOKEN_LBRACE", "TOKEN_RBRACE",    "TOKEN_SEMICOLON",
      "TOKEN_INT",    "TOKEN_NULL",   "TOKEN_PRIM_TYPE", "TOKEN_STATEMENT",
      "TOKEN_COMMA",  "TOKEN_EQUALS", "TOKEN_PLUS",      "TOKEN_MINUS",
      "TOKEN_STAR",   "TOKEN_SLASH",
  };
  return names[type];
}
