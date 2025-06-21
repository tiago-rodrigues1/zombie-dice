#include "ini_parser.hpp"
#include "zdie.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

// void printConfig(const std::map<std::string, std::map<std::string, std::string>>& config) {
//     for (const auto& outerPair : config) {
//         const std::string& outerKey = outerPair.first;
//         const auto& innerMap = outerPair.second;

//         std::cout << outerKey << ":\n";

//         for (const auto& innerPair : innerMap) {
//             std::cout << "  " << innerPair.first << " = " << innerPair.second << "\n";
//         }
//     }
// }

int main() {

	
	std::ifstream ifs{ "zdice.ini" };
	
  IniParser::parse(ifs);

	// Zdie dice(GREEN, config["Dice"]["weak_die_faces"]);
	
	// for (int i = 0; i < 6; ++i) {
	// 	std::cout << dice.roll() << '\n';
	// }

  // printConfig(config);

  std::cout << IniParser::get_config<int>("max_turns") << '\n';
  std::cout << IniParser::get_config<std::string>("Dice.tough_die_faces") << '\n';
  std::cout << IniParser::get_config<int>("Game.brains_to_win") << '\n';
  std::cout << IniParser::get_config<double>("Game.max_players") << '\n';

  return 0;
}