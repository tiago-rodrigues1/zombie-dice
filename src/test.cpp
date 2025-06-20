#include "ini_parser.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "game_controller.hpp"

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
    GameController gc;
    gc.welcome_message();

    gc.define_players(gc.read_players());
    gc.define_first_player();
    gc.players_message();
  
}