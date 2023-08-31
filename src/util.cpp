#include "util.hpp"

int isType(std::string str) {
    if (str == "void" || str == "int" || str == "str" || str == "const") return 1;
    else return 0;
}
