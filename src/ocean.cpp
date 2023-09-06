#include "ocean.hpp"
#include "asm.hpp"
#include "ast.hpp"
#include "io.hpp"
#include "parser.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

std::string oceanCompile(std::string path, int ismainfile = 0) {
  std::string program = readFile(path);

  Scope *globalScope = new Scope();
  Parser *parser = new Parser(program);
  AST *root = parser->parse(globalScope);
  // std::cout << "\nDONE COMPILING AST ROOT...\n" << std::endl;
  // printAST(root);
  AsmFrontend *asmFrontend = new AsmFrontend(root);
  // std::cout << "\nDONE COMPILING TO ASM...\n" << std::endl;
  std::string compiled = asmFrontend->compileRootCompound(root, ismainfile);

  // std::cout << compiled << std::endl;

  return compiled; // TODO: replace with final asm
}
