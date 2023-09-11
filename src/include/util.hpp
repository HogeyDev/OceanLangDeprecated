#pragma once
#include "token.hpp"
#include <string>

int isType(std::string str);
int isBinaryOperator(token_T tokenType);
std::string asmComment(const std::string &comment);
