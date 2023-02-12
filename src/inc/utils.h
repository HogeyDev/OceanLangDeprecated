#pragma once

#include "list.h"
#include "token.h"

void assert_not_null(void* x);
char* str_to_hex(const char* instr);
list_T* str_to_hex_chunks(const char* instr);
char* mkstr(const char* str);
char* str_format(char* instr);
char str_to_escape_seq(const char* instr);
unsigned int is_datatype(token_T* token);
char* get_string_contents(char* str);
