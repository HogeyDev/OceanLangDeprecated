#include "io.hpp"
//#include <iostream>
#include <fstream>
#include <string>

std::string readFile(std::string path) {
    std::string contents;
    std::string line;
    std::ifstream file(path);

    while (std::getline(file, line)) {
        contents.append(line);
        contents.append("\n");
    }

    file.close();

    return contents;
}

void writeFile(std::string path, std::string contents) {
    std::ofstream outputFile(path);
    outputFile << contents;
    outputFile.close();
}
