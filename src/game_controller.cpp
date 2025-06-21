#include "game_controller.hpp"

#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <chrono>


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
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::shuffle(players.begin(), players.end(), std::default_random_engine(seed));

  current_player = players[0];
}

void GameController::players_message() {
  std::cout << ">>> The players of the game are:\n";
  for (Player player : players) {
    std::cout << "\"" << player.name << "\"\n";
  }

  std::cout << ">>> The player who will start the game is \"" << current_player.name << "\"\n";
    "Press <Enter> to start the match.\n";
}

void GameController::read_actions(){
  std::string action;
  std::getline(std::cin, action);

  if (action == "H"){
    current_player.current_action = actions_e::HOLD;
  } else if (action == "Q"){
    current_player.current_action = actions_e::QUIT_TURN;
  } else if (action.empty()){
    current_player.current_action = actions_e::ROLL;
  }
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
  case GameState::READ_ACTION:
    read_actions();
    current_player.apply_action();
  default:
    break;
  }
}

void GameController::update(){
  switch (game_state)
  {
  case GameState::WELCOME:
    game_state = GameState::READ_PLAYERS;
    break;
  case GameState::READ_PLAYERS:
    game_state = GameState::DEFINE_FIRST_PLAYER;
    break;
  case GameState::DEFINE_FIRST_PLAYER:
    game_state = GameState::READ_ACTION;
    break;
  default:
    break;
  }
}

void title_and_message_area(){
  std::cout << std::setw(20) << "→☣️ [🧟] Zombie Dice Delux, v 0.1 [🧟] ☣️←"  << std::setw(20) << "\n";
  std::cout << std::setw(20) << "┌────────────────────────┐" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "│      Global Score      │" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "└────────────────────────┘" << std::setw(20) << "\n";

  // terminar

}

void GameController::parse_config(int argc, char* argv[]) {
  // alguma coisa
}