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

void GameController::print_state() {
  std::cout << ">>> GAME_STATE = " << to_string(game_state) << '\n';
}

void GameController::update_player() {
  ++current_player_idx;

  if (current_player_idx >= players.size()) {
    current_player_idx = 0;
  }
}

std::vector<std::string> GameController::read_players() {
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
    partial_points[player.name] = 0;
    players.push_back(player);
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

  process_events();
}

void GameController::roll_dices() {
  size_t i = 0;
  while (i < DRA.size()) {
    Zdie dice = DRA[i];
    ZdieFaces face = dice.roll();

    if (face == ZdieFaces::BRAIN) {
      BSA.push_back(dice);
      DRA.erase(DRA.begin() + i);
    } else if (face == ZdieFaces::SHOTGUN) {
      SSA.push_back(dice);
      DRA.erase(DRA.begin() + i);
    } else {
      ++i;
    }
  }

  if (SSA.size() >= 3) {
    game_state = GameState::LOSE_TURN;
    // dice_bag.add_dices(BSA);
    // dice_bag.add_dices(SSA);
    // dice_bag.add_dices(DRA);
    // SSA.clear();
    // BSA.clear();
    // DRA.clear();
    process_events();
  }
}

void GameController::handle_roll() {
  size_t dra_dices_count{ DRA.size() };
  size_t required_dices{ 3 };

  if (dra_dices_count < required_dices) {
    required_dices -= dra_dices_count;
    
    if (dice_bag.count_dices() < required_dices) {
      game_state = GameState::RECYCLE;
      process_events();
    }
    
    std::vector<Zdie> drawed_dices{ dice_bag.draw(required_dices) };
    DRA.insert(DRA.begin(), drawed_dices.begin(), drawed_dices.end());
  }

  roll_dices();
}

void GameController::recycle() {
  std::string current_player{ players[current_player_idx].name };
  partial_points[current_player] = BSA.size();

  std::vector<Zdie> recycle_dices{ BSA };
  dice_bag.add_dices(recycle_dices);

  BSA.clear();
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
    handle_roll();
    break;
  case GameState::RECYCLE:
    recycle();
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
  case GameState::RECYCLE:
    game_state = ROLL;
    break;
  case GameState::END_TURN:
    game_state = GameState::READ_ACTION;
    break;
  default:
    break;
  }
}

bool GameController::game_over(bool quit_game) {
  return game_state == LOSE_TURN;
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

  RunningOpt run_options;

  if (IniParser::parse(arg)) {
    run_options.weak_dice = IniParser::get_config<int>("Game.weak_dice", run_options.weak_dice);
    run_options.strong_dice = IniParser::get_config<int>("Game.strong_dice", run_options.strong_dice);
    run_options.tough_dice = IniParser::get_config<int>("Game.tough_dice", run_options.tough_dice);
    run_options.max_players = IniParser::get_config<int>("Game.max_players", run_options.max_players);
    run_options.brains_to_win = IniParser::get_config<int>("Game.brains_to_win", run_options.brains_to_win);
    run_options.max_turns = IniParser::get_config<int>("Game.max_turns", run_options.max_turns);

    run_options.weak_die_faces = IniParser::get_config<std::string>("Dice.weak_die_faces", run_options.weak_die_faces);
    run_options.strong_die_faces = IniParser::get_config<std::string>("Dice.strong_die_faces", run_options.strong_die_faces);
    run_options.tough_die_faces = IniParser::get_config<std::string>("Dice.tough_die_faces", run_options.tough_die_faces);
  } else {
    std::cout << "--> using default config\n";
  }

  setup(run_options);
}