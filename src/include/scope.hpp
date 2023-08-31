#include <string>
#include <vector>
#include <iostream>

class Scope {
    std::vector<std::vector<std::string>> variables;
    public:
        Scope() {}
        void addNewVariable(std::string type, std::string name, std::string value) {
            // this->variables.emplace_back(type, name, value);
            this->variables.push_back(std::vector<std::string>{type, name, value});
        }
        void addNewVariable(std::string type, std::string name, int value) {
            // this->variables.emplace_back(type, name, std::to_string(value));
            this->variables.push_back(std::vector<std::string>{type, name, std::to_string(value)});
        }
        int getVariableOffset(std::string variableName) {
            for (unsigned int i = 0; i < this->variables.size(); i++) {
                if (this->variables[i][1] == variableName)
                    return (i + 1) * 8;
            }
            std::cout << "[SCOPE]: No Variable Called '" << variableName << "'" << std::endl;
            exit(1);
        }
};
