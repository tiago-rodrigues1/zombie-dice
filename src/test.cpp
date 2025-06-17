#include "ini_parser.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

void printConfig(const std::map<std::string, std::map<std::string, std::string>>& config) {
    for (const auto& outerPair : config) {
        const std::string& outerKey = outerPair.first;
        const auto& innerMap = outerPair.second;

        std::cout << outerKey << ":\n";

        for (const auto& innerPair : innerMap) {
            std::cout << "  " << innerPair.first << " = " << innerPair.second << "\n";
        }
    }
}

int main() {

  std::ifstream ifs{ "zdice.ini" };

  auto config = IniParser::parse(ifs);
  printConfig(config);
  return 0;
}