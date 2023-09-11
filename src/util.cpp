#include "util.hpp"
#include "config.hpp"
#include <iostream>

int isType(std::string str) {
  if (str == "void" || str == "int" || str == "str" || str == "const")
    return 1;
  else
    return 0;
}

int isBinaryOperator(token_T tokenType) {
  switch (tokenType) {
  case TOKEN_PLUS:
  case TOKEN_MINUS:
  case TOKEN_STAR:
  case TOKEN_SLASH:
    return 1;
  default:
    return 0;
  }
}

std::string asmComment(const std::string &comment) {
#ifdef ASM_COMMENTS
  // std::cout << "COMMENTING" << std::endl;
  return ("; " + comment + "\n");
#else
  // std::cout << "SKIPPING ASM COMMENT: " << comment << std::endl;
  return "";
#endif
}
