#include "ini_parser.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "game_controller.hpp"
#include <iomanip>

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

void title_and_message_area(){
  std::cout << std::setw(20) << "→☣️ [🧟] Zombie Dice Delux, v 0.1 [🧟] ☣️←"  << std::setw(20) << "\n";
  std::cout << std::setw(20) << "┌────────────────────────┐" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "│      Global Score      │" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "└────────────────────────┘" << std::setw(20) << "\n";



}


int main() {
    title_and_message_area();
  
}