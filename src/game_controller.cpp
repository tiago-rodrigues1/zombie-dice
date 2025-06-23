#include "common.hpp"
#include "game_controller.hpp"
#include "ini_parser.hpp"
#include "views.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>

void GameController::print_state() {
  std::cout << ">>> GAME_STATE = " << to_string(game_state) << '\n';
}

void GameController::update_player() {
  ++current_player_idx;

  if (current_player_idx >= players.size()) {
    current_player_idx = 0;
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
  std::cout << "> READING PLAYERS\n";
  std::string names_str;
  std::vector<std::string> names;
  std::getline(std::cin, names_str);

  names = split(names_str, ',', true);

  if (names.size() < min_players) {
    std::cout
      << "Please, enter at least " << min_players << " names of the players in a single line, separated by comma. \n"
         ">>> ";
    return read_players();
  }

  if (names.size() > max_players) {
    names.erase(names.begin() + max_players, names.end());
  }

  shuffler(names.begin(), names.end());

  return names;
}

void GameController::define_players(std::vector<std::string> players_names) {
  for (const std::string& name : players_names) {
    Player player;
    player.name = name;
    players.push_back(player);
  }

  for (const Player& p : players) {
    std::cout << p.name << '\n';
  }

  current_player_idx = 0;
}

void GameController::points_to_player() {
  if (!BSA.empty()) {
    players[current_player_idx].points += BSA.size();
  }
}

void GameController::read_actions() {
  std::string action;
  std::getline(std::cin, action);

  action = trim(action);

  if (action == "h" or action == "H") {
    game_state = GameState::HOLD;
  } else if (action == "q" or action == "Q") {
    game_state = GameState::QUIT;
  } else if (action.empty()) {
    game_state = GameState::ROLL;
  }
}

void GameController::render() {
  switch (game_state) {
    case GameState::WELCOME:
      Views::welcome_message(min_players, max_players);
      break;
    case GameState::START:
      Views::show_players_message(players);
      break;
  }
}

void GameController::process_events() {
  switch (game_state) {
  case GameState::READ_PLAYERS:
    define_players(read_players());
    break;
  case GameState::READ_ACTION:
    read_actions();
    break;
  case GameState::ROLL:
    // DRA = dice_bag.draw();
    // roll_dices();
    std::cout << "ROLL\n";
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
  case GameState::NEUTRAL:
    game_state = WELCOME;
    break;
  case GameState::WELCOME:
    game_state = GameState::READ_PLAYERS;
    break;
  case GameState::READ_PLAYERS:
    game_state = GameState::START;
    break;
  case GameState::START:
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

bool GameController::game_over(bool quit_game) {
  current_player_idx += 1;
  return current_player_idx == 2;
}

void GameController::setup(const RunningOpt& run_options) {
  dice_bag.add_dices({GREEN, run_options.weak_die_faces}, run_options.weak_dice);
  dice_bag.add_dices({ORANGE, run_options.strong_die_faces}, run_options.strong_dice);
  dice_bag.add_dices({RED, run_options.tough_die_faces}, run_options.tough_dice);
}

void GameController::parse_config(int argc, char* argv[]) {
  if (argc == 1) {
    return;
  }
  
  std::string arg{ argv[1] };

  if (arg == "-h") {
    std::cout << "usage\n";
    exit(EXIT_SUCCESS);
  }

  std::ifstream ifs{ arg };

  RunningOpt run_options;

  if (ifs.good()) {
    IniParser::parse(ifs);

    run_options.weak_dice = IniParser::get_config<int>("Game.weak_dice");
    run_options.strong_dice = IniParser::get_config<int>("Game.strong_dice");
    run_options.tough_dice = IniParser::get_config<int>("Game.tough_dice");
    run_options.max_players = IniParser::get_config<int>("Game.max_players");
    run_options.brains_to_win = IniParser::get_config<int>("Game.brains_to_win");
    run_options.max_turns = IniParser::get_config<int>("Game.max_turns");

    run_options.weak_die_faces = IniParser::get_config<std::string>("Dice.weak_die_faces");
    run_options.strong_die_faces = IniParser::get_config<std::string>("Dice.strong_die_faces");
    run_options.tough_die_faces = IniParser::get_config<std::string>("Dice.tough_die_faces");
  }
}