#include "ini_parser.hpp"
#include "zdie.hpp"
#include "dice_bag.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "game_controller.hpp"
#include <iomanip>
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

// void title_and_message_area(){
//   std::cout << std::setw(20) << "→☣️ [🧟] Zombie Dice Delux, v 0.1 [🧟] ☣️←"  << std::setw(20) << "\n";
//   std::cout << std::setw(20) << "┌────────────────────────┐" << std::setw(20) << "\n";
//   std::cout << std::setw(20) << "│      Global Score      │" << std::setw(20) << "\n";
//   std::cout << std::setw(20) << "└────────────────────────┘" << std::setw(20) << "\n";



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