#include <cstdlib>
#include <iostream>
#include <string>
#include "ocean.hpp"
#include "asm.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "\033[1;31mError\033[1;0m: \n\tNo input files specified" << std::endl;
        exit(1);
    } else {
        // run compilation code
        std::string fullAsm = oceanCompile(argv[1], 1);
        writeFile("test/testFile.asm", fullAsm);
        system("nasm -g -f elf64 test/testFile.asm -o test/testFile.o");
        system("ld -m elf_x86_64 test/testFile.o -o test/testFile -e main");
    }
    return 0;
}
