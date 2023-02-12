#pragma once

#include "parser.h"

typedef struct PREPROCESS_STRUCT {
	enum PROCESS_ENUM {
		PREPROCESS_IMPORT,
	} processes;

	AST_T* ast;

	// import stuff
	char* filepath;
} preprocess_T;

preprocess_T* init_preprocess(AST_T* ast);

AST_T* preprocessor_preprocess(preprocess_T* preprocessor);
