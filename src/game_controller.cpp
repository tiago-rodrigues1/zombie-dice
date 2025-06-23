#include "game_controller.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void GameController::update_player() {
  ++players_count;

  if (players_count >= players.size()) {
    players_count = 0;
  }
}

void GameController::roll_dices() {
  for (size_t i{ DRA.size() }; i > 0; --i) {
    Zdie dice = DRA[i];
    char face = dice.roll();

    if (face == 'b') {
      BSA.push_back(dice);
      DRA.erase(DRA.begin() + i);
    } else if (face == 'f') {
      SSA.push_back(dice);
      DRA.erase(DRA.begin() + i);
    }
  }

  if (SSA.size() >= 3) {
    game_state = GameState::END_TURN;
    dice_bag.add_dices(BSA);
    dice_bag.add_dices(SSA);
    dice_bag.add_dices(DRA);
    SSA.clear();
    BSA.clear();
    DRA.clear();
    process_events();
  }
}

std::vector<std::string> GameController::read_players() {
  std::string players_name;
  std::vector<std::string> players;
  std::getline(std::cin, players_name);

  std::stringstream ss(players_name);
  std::string name;

  while (std::getline(ss, name, ',')) {
    players.push_back(name);
  }

  if (players.size() < 2) {
    std::cout
      << "Please, enter at least 2 names of the players in a single line, separated by comma. \n"
         ">>> ";
    return read_players();
  }

  if (players.size() > 6) {
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

void GameController::points_to_player() {
  if (!BSA.empty()) {
    players[players_count].points += BSA.size();
  }
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
  players_count = 0;
}

void GameController::players_message() {
  std::cout << ">>> The players of the game are:\n";
  for (Player player : players) {
    std::cout << "\"" << player.name << "\"\n";
  }

  std::cout << ">>> The player who will start the game is \"" << players[players_count].name << "\"\n";
  "Press <Enter> to start the match.\n";
}

void GameController::read_actions() {
  std::string action;
  std::getline(std::cin, action);

  if (action == "h") {
    game_state = GameState::END_TURN;
  } else if (action == "q") {
    game_over(true);
  } else if (action.empty()) {
    game_state = GameState::ROLL;
  }
}

void GameController::process_events() {
  switch (game_state) {
  case GameState::WELCOME:
    welcome_message();
    break;
  case GameState::READ_PLAYERS:
    define_players(read_players());
    define_first_player();
    players_message();
    break;
  case GameState::READ_ACTION:
    read_actions();
    process_events();
    break;
  case GameState::ROLL:
    ++players[players_count].count_turns;
    DRA = dice_bag.draw();
    roll_dices();
    // falta terminar
    break;
  case GameState::END_TURN:
    points_to_player();
    update_player();
  default:
    break;
  }
}

void GameController::update() {
  switch (game_state) {
  case GameState::WELCOME:
    game_state = GameState::READ_PLAYERS;
    break;
  case GameState::READ_PLAYERS:
    game_state = GameState::READ_ACTION;
    break;
  case GameState::ROLL:
    game_state = GameState::READ_ACTION;
    break;
  case GameState::END_TURN:
    game_state = GameState::READ_ACTION;
    break;
  default:
    break;
  }
}

void title_and_message_area() {
  std::cout << std::setw(20) << "→☣️ [🧟] Zombie Dice Delux, v 0.1 [🧟] ☣️←" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "┌────────────────────────┐" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "│      Global Score      │" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "└────────────────────────┘" << std::setw(20) << "\n";

  // terminar
}

bool GameController::checks_if_all_player_had_the_same_turns(){
  for(Player player : players){
    player.count_turns
  }
}

bool GameController::game_over(bool quit_game) {
  
  if((players[players_count].count_turns >= 13 &&) || !quit_game){
    return true;
  }
  
  return false;
}

void GameController::parse_config(int argc, char* argv[]) {
  // alguma coisa
}