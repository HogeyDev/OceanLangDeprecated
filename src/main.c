#include "include/ocean.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc < 2) return 1;

	compile_file(argv[1]);
	return 0;
}
