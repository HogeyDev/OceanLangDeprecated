#pragma once

#include <iostream>
#include <string>
#include <vector>

class Scope {
  // std::vector<std::vector<std::string>> strings;
  // std::vector<std::vector<std::string>> ints;
  std::vector<std::vector<std::string>> variables;

public:
  Scope() {}
  void addNewVariable(std::string type, std::string name, std::string value) {
    // this->variables.emplace_back(type, name, value);
    this->variables.push_back(std::vector<std::string>{type, name, value});
  }
  void addNewVariable(std::string type, std::string name) {
    // this->variables.emplace_back(type, name, value);
    this->variables.push_back(std::vector<std::string>{
        type, name, "STR" + std::to_string(this->getNumStrings())});
  }
  int getNumStrings() {
    int ret = 0;
    for (auto &s : this->variables) {
      if (s[0] == "str")
        ret++;
    }
    return ret;
  }
  int getNumInts() {
    int ret = 0;
    for (auto &s : this->variables) {
      if (s[0] == "int")
        ret++;
    }
    return ret;
  }
  void addNewVariable(std::string type, std::string name, int value) {
    // this->variables.emplace_back(type, name, std::to_string(value));
    this->variables.push_back(
        std::vector<std::string>{type, name, std::to_string(value)});
  }
  int getVariableOffset(std::string variableName) {
    int totLen = this->variableListLength();
    for (unsigned int i = 0; i < this->variables.size(); i++) {
      if (this->variables[i][1] == variableName)
        return (totLen - i) * 8;
    }
    std::cout << "[SCOPE]: No Variable Called '" << variableName << "'"
              << std::endl;
    exit(1);
  }
  int variableListLength() { return this->variables.size(); }
  std::vector<std::vector<std::string>> getFullVariableList() {
    std::vector<std::vector<std::string>> ret;
    for (auto &s : this->variables) {
      ret.push_back(s);
    }
    return ret;
  }
  void printVariableList() {
    std::cout << "<";
    for (auto &s : this->variables) {
      std::cout << "<" << s[0] << ", " << s[1] << ", " << s[2] << ">";
    }
    std::cout << ">" << std::endl;
  }
  void printVariableList(std::vector<std::vector<std::string>> list) {}
};
