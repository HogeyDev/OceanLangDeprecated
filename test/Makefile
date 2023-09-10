all: compile run

compile:
	nasm -g -f elf64 testFile.asm -o testFile.o
	ld -m elf_x86_64 testFile.o -o testFile -e main

run:
	./testFile

debug:
	gdb ./testFile
