#include <iostream>
#include <string>
#include <vector>

class Scope {
  std::vector<std::vector<std::string>> strings;
  std::vector<std::vector<std::string>> ints;

public:
  Scope() {}
  void addNewVariable(std::string type, std::string name, std::string value) {
    // this->variables.emplace_back(type, name, value);
    this->strings.push_back(std::vector<std::string>{type, name, value});
  }
  void addNewVariable(std::string type, std::string name, int value) {
    // this->variables.emplace_back(type, name, std::to_string(value));
    this->ints.push_back(
        std::vector<std::string>{type, name, std::to_string(value)});
  }
  int getVariableOffset(std::string variableName) {
    int totLen = strings.size() + ints.size();
    for (unsigned int i = 0; i < this->strings.size(); i++) {
      if (this->strings[i][1] == variableName)
        return (totLen - i) * 8;
    }
    for (unsigned int i = 0; i < this->ints.size(); i++) {
      if (this->ints[i][1] == variableName)
        return (totLen - i) * 8;
    }
    std::cout << "[SCOPE]: No Variable Called '" << variableName << "'"
              << std::endl;
    exit(1);
  }
  int variableListLength() {
    return (this->strings.size() + this->ints.size());
  }
};
