#pragma once

typedef enum {
	DATA_TYPE_INT = 3,
	DATA_TYPE_CHAR,
	DATA_TYPE_STR,
	DATA_TYPE_ARRAY,
	DATA_TYPE_UNKNOWN,
} data_type;

int typename_to_int(const char* name);

char* datatype_to_int(int datatype);
