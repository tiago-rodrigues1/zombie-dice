#include "ini_parser.hpp"
#include "zdie.hpp"
#include "dice_bag.hpp"

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

	
	// std::ifstream ifs{ "zdice.ini" };
	
  // IniParser::parse(ifs);

	Zdie g_dice(GREEN, "bbbffs");
  Zdie r_dice(RED, "bbffss");
  Zdie o_dice(ORANGE, "bffsss");
	
	// for (int i = 0; i < 6; ++i) {
	// 	std::cout << dice.roll() << '\n';
	// }

  // printConfig(config);

  // std::cout << IniParser::get_config<int>("max_turns") << '\n';
  // std::cout << IniParser::get_config<std::string>("Dice.tough_die_faces") << '\n';
  // std::cout << IniParser::get_config<int>("Game.brains_to_win") << '\n';
  // std::cout << IniParser::get_config<double>("Game.max_players") << '\n';


  DiceBag db;

  db.add_dices({g_dice, g_dice, g_dice, o_dice, o_dice, o_dice, r_dice, r_dice, r_dice});

  std::cout << "Dices count: " << db.count_dices() << '\n';

  for (auto d : db.draw()) {
    std::cout << d.roll() << '\n';
  }

  std::cout << "Dices count: " << db.count_dices() << '\n';

  return 0;
}