#include "include/types.h"
#include <string.h>

int typename_to_int(const char* name) {
	if (strcmp(name, "int") == 0) return DATA_TYPE_INT;
	if (strcmp(name, "char") == 0) return DATA_TYPE_CHAR;
	if (strcmp(name, "str") == 0) return DATA_TYPE_STR;

	return DATA_TYPE_UNKNOWN;
}

char* datatype_to_int(int datatype) {
	switch (datatype) {
		case DATA_TYPE_CHAR: return "DATA_TYPE_CHAR";
		case DATA_TYPE_INT: return "DATA_TYPE_INT";
		case DATA_TYPE_STR: return "DATA_TYPE_STR";
		case DATA_TYPE_UNKNOWN: return "DATA_TYPE_UNKNOWN";
		default: return "not_stringable";
	}
}
