#include "game_controller.hpp"

#include <iostream>
#include <random>
#include <sstream>

std::vector<std::string> DRA::roll_dices() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<std::string> chosen_faces;
  std::vector<Dice> remaining_dices;

  for (size_t i{ 0 }; i < chosen_dices.size(); ++i) {
    std::uniform_int_distribution<> dist(0, 5);
    auto index = dist(gen);

    std::string face(1, chosen_dices[i].faces[index]);

    // removemos aqueles que são brains e gun
    if (face == "s") {
      remaining_dices.push_back(chosen_dices[i]);
    }

    chosen_faces.push_back(face);
  }

  chosen_dices = std::move(remaining_dices);

  return chosen_faces;
}

// void DiceBag::get_dices(Player& player) {
// std::random_device rd;
// std::mt19937 gen(rd());
// std::vector<size_t> weights;

// for (const auto& pair : dice_bag) {
// size_t count = pair.second;
// weights.push_back(count);
//}

// for (int i{ 0 }; i < 3; ++i) {

// std::discrete_distribution<> dist(weights.cbegin(), weights.cend());
// auto index = dist(gen);

// auto it = dice_bag.begin();
// std::advance(it, index);
// player.player_DRA.chosen_dices.push_back(it->first);
// }
//}

std::vector<std::string> GameController::read_players() {
  std::string players_name;
  std::vector<std::string> players;
  std::getline(std::cin, players_name);

  std::stringstream ss(players_name);
  std::string name;

  while (std::getline(ss, name, ',')) {
    players.push_back(name);
  }

  if(players.size() < 2){
    std::cout << "Please, enter at least 2 names of the players in a single line, separated by comma. \n"
    ">>> ";
    return read_players();
  }

  if(players.size() > 6){
    players.erase(players.begin() + 6, players.end());
  }

  return players;
}

void GameController::welcome_message() {

  std::cout << "         ---> Welcome to the Zombi Dice game (v 0.1) <-- \n"
               "-- copyright DIMAp/UFRN 2024-2025 -- \n"

               "\nThe object of the jeopardy dice game Zombie Dice is to be the\n"
               "first to eat 13 or more human brains in a turn.\n"
               "Each player's turn consists of repeatedly rolling 3 dice.\n"
               "A die may produce 3 outcomes: you ate a brain, the human escaped,\n"
               "or you were shot!\n"
               "If you get shot 3 or more times you die and loose all brains you\n"
               "ate in that turn; then the turn goes to the next player.\n"
               "However, if you decide to stop before get shot 3 times, you keep\n"
               "the brains you ate.\n"

               "\nBefore we begin, please enter the names of the players\n"
               "in a single line, separated by comma. For example:\n"
               ">>> Jack, Menace, Ultraz, Boris\n"

               "\nMinimum of 2 player, maximum of 6 names.\n"
               "Providing an empty line will end the game.\n"
               "\n>>> ";
}

void GameController::define_players(std::vector<std::string> players_name) {


  for (const std::string& name : players_name) {
    Player player;
    player.name = name;
    players.push_back(player);
  }
}

void GameController::define_first_player() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, players.size() - 1);
  auto index = dist(gen);

  current_player = players[index];
}

void GameController::players_message() {
  std::cout << ">>> The players of the game are:\n";
  for (Player player : players) {
    std::cout << "\"" << player.name << "\"\n";
  }

  std::cout << ">>> The player who will start the game is \"" << current_player.name << "\"\n";
  std::cout << "Press <Enter> to start the match.\n";
}

void GameController::process_events() {
  switch (game_state) {
  case GameState::WELCOME:
    welcome_message();
    break;
  case GameState::READ_PLAYERS:
    define_players(read_players());
    break;
  case GameState::DEFINE_FIRST_PLAYER:
    define_first_player();
    players_message();
    break;

  default:
    break;
  }
}

void GameController::parse_config(int argc, char* argv[]) {
  // alguma coisa
}