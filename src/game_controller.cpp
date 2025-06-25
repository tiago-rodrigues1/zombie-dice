#include "include/game_controller.hpp"
#include "include/common.hpp"
#include "include/ini_parser.hpp"
#include "include/views.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void GameController::next_player() {
  ++current_player_idx;

  if (current_player_idx >= players.size()) {
    current_player_idx = 0;
  }

  ++players[current_player_idx].count_turns;
}

std::vector<std::string> GameController::read_players() {
  std::string names_str;
  std::vector<std::string> names;
  std::getline(std::cin, names_str);

  names = split(names_str, ',', true);

  if (names.size() < min_players) {
    std::cout << "Please, enter at least " << min_players
              << " names of the players in a single line, separated by comma. \n"
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
  players[current_player_idx].count_turns = 0;
}

void GameController::consolidate_points() {
  Player& player{ players[current_player_idx] };
  player.points += partial_points[player.name];

  if (player.points >= highest_point && limit_of_turns == 0) {
    limit_of_turns = player.count_turns;
  }
}

void GameController::read_actions() {
  message = {
    "Ready to play?", "<enter> - roll dices", "H + <enter> - hold turn", "Q + <enter> - quit game"
  };
  prev_dra.clear();

  std::string action;
  std::getline(std::cin, action);

  action = trim(action);

  if (action == "h" or action == "H") {
    game_state = GameState::HOLD;
  } else if (action == "q" or action == "Q") {
    game_state = GameState::QUIT;
  } else if (action.empty()) {
    game_state = GameState::ROLL;
  } else {
    game_state = GameState::ERROR;
  }

  process_events();
}

void GameController::roll_dices() {
  size_t i = 0;
  while (i < DRA.size()) {
    Zdie dice = DRA[i];
    ZdieFaces face = dice.roll();

    prev_dra.push_back(dice);

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
    process_events();
  }
}

void GameController::dices_back_to_bag() {
  dice_bag.add_dices(BSA);
  dice_bag.add_dices(SSA);
  dice_bag.add_dices(DRA);
  SSA.clear();
  BSA.clear();
  DRA.clear();
}

void GameController::end_turn() {
  consolidate_points();
  dices_back_to_bag();
  next_player();
}

void GameController::handle_roll() {
  message = { "Rolling outcome:",
              "# brains you ate: " + std::to_string(BSA.size()),
              "# shots that hit you: " + std::to_string(SSA.size()),
              "Press <enter> to continue." };
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

void GameController::handle_hold() {
  std::string player{ players[current_player_idx].name };

  partial_points[player] += BSA.size();
}

void GameController::recycle() {
  std::string current_player{ players[current_player_idx].name };
  partial_points[current_player] = BSA.size();

  std::vector<Zdie> recycle_dices{ BSA };
  dice_bag.add_dices(recycle_dices);

  BSA.clear();
}

void GameController::handle_quit() {
  std::cout << players[current_player_idx].name << " is quitting\n";
  dices_back_to_bag();
  players.erase(players.begin() + current_player_idx);

  if (players.size() == 1) {
    game_state = END_GAME;
  }

  next_player();
}

void GameController::render() {
  switch (game_state) {
  case GameState::WELCOME:
    Views::welcome_message(min_players, max_players);  // Essa já foi traduzida na classe Views
    break;
  case GameState::START:
    Views::show_players_message(players);  // Essa também já está em português
    break;
  case GameState::READ_ACTION:
    Views::areas(players[current_player_idx], dice_bag.count_dices());
    Views::rolling_table(prev_dra, BSA, SSA);
    Views::message_area(message);
    break;
  case GameState::ROLL:
    Views::areas(players[current_player_idx], dice_bag.count_dices());
    Views::message_area(message);
    break;
  case GameState::END_TURN:
    Views::title_and_scoreboard_area(players, current_player_idx);
    Views::areas(players[current_player_idx], dice_bag.count_dices());
    Views::message_area({ "Ready to play?",
                          "<enter> - roll dices",
                          "H + <enter> - hold turn",
                          "Q + <enter> - quit game" });
    Views::rolling_table(prev_dra, BSA, SSA);
    prev_dra.clear();
    Views::areas(players[current_player_idx], dice_bag.count_dices());
    Views::message_area(message);
    break;
  case GameState::ERROR:
    Views::message_area(message);
    break;
  case GameState::QUIT:
    break;

  case GameState::HOLD:
    break;
  case GameState::END_GAME:
    break;
  case GameState::LOSE_TURN:
    break;

  case GameState::RECYCLE:
    break;
  case GameState::READ_PLAYERS:
    break;
  case GameState::NEUTRAL:
    break;
  }
}

std::vector<Player> GameController::get_highest_players() {
  auto it = std::max_element(players.begin(), players.end(), [](const Player& a, const Player& b) {
    return a.points < b.points;
  });

  if (it != players.end()) {
    highest_point = it->points;

    std::vector<Player> top_players;
    for (const Player& player : players) {
      if (player.points == highest_point) {
        top_players.push_back(player);
      }
    }

    return top_players;
  }

  return {};
}

void GameController::checks_end_of_game() {

  std::vector<Player> top_players = get_highest_players();

  if (top_players.size() == 1) {
    winner = top_players[0];
    std::cout << "We have a winner: " << winner.name << "\n";
    game_state = GameState::END_GAME;
  }

  else if (!top_players.empty()) {
    std::string player_names;
    for (Player player : top_players) {
      player_names.append("\"");
      player_names.append(player.name);
      player_names.append("\" ");
    };
    message
      = { "We have a tie!", "Players in tie break:", player_names, "Press <enter> to continue." };
    game_state = GameState::READ_ACTION;
    limit_of_turns++;
    players = top_players;
    current_player_idx = 0;
  }
}

bool GameController::player_can_play() {
  return limit_of_turns == 0 || players[current_player_idx].count_turns != limit_of_turns;
}

void GameController::process_events() {
  switch (game_state) {
  case GameState::READ_PLAYERS:
    define_players(read_players());
    break;
  case GameState::READ_ACTION:
    message = {
      "Ready to play?", "<enter> - roll dices", "H + <enter> - hold turn", "Q + <enter> - quit game"
    };
    read_actions();
    break;
  case GameState::ROLL:
    if (player_can_play()) {
      handle_roll();
    } else {
      if (all_turns_completed()) {
        checks_end_of_game();
      } else {
        message = { "All turns completed for this player." };
        game_state = GameState::READ_ACTION;
        next_player();
      }
    }
    break;
  case GameState::RECYCLE:
    recycle();
    break;
  case GameState::HOLD:
    handle_hold();
    break;
  case GameState::LOSE_TURN:
    dices_back_to_bag();
    break;
  case GameState::END_TURN:
    end_turn();
    break;
  case GameState::QUIT:
    handle_quit();
    break;
  case GameState::ERROR:
    message = { "Invalid input!", "Please select a valid option" };
    break;
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
  case GameState::HOLD:
    game_state = END_TURN;
    break;
  case GameState::QUIT:
    game_state = READ_ACTION;
    break;
  case GameState::LOSE_TURN:
    game_state = END_TURN;
    break;
  case GameState::RECYCLE:
    game_state = ROLL;
    break;
  case GameState::END_TURN:
    game_state = GameState::READ_ACTION;
    break;
  case GameState::ERROR:
    game_state = GameState::READ_ACTION;
    break;
  default:
    break;
  }
}

bool GameController::all_turns_completed() {
  for (Player player : players) {
    if (limit_of_turns != player.count_turns) {
      return false;
    }
  }

  return true;
}

bool GameController::game_over() { return game_state == GameState::END_GAME; }

void GameController::setup(const RunningOpt& run_options) {
  dice_bag.add_dices({ GREEN, run_options.weak_die_faces }, run_options.weak_dice);
  dice_bag.add_dices({ ORANGE, run_options.strong_die_faces }, run_options.strong_dice);
  dice_bag.add_dices({ RED, run_options.tough_die_faces }, run_options.tough_dice);
}

void GameController::parse_config(int argc, char* argv[]) {
  RunningOpt run_options;

  if (argc == 1) {
    std::cerr << "[warning] no .ini file passed\n--> using default config\n";
  } else {
    std::string arg{ argv[1] };

    if (arg == "-h") {
      Views::usage();
      exit(EXIT_SUCCESS);
    }

    if (IniParser::parse(arg)) {
      run_options.weak_dice = IniParser::get_config<int>("Game.weak_dice", run_options.weak_dice);
      run_options.strong_dice
        = IniParser::get_config<int>("Game.strong_dice", run_options.strong_dice);
      run_options.tough_dice
        = IniParser::get_config<int>("Game.tough_dice", run_options.tough_dice);
      run_options.max_players
        = IniParser::get_config<int>("Game.max_players", run_options.max_players);
      run_options.brains_to_win
        = IniParser::get_config<int>("Game.brains_to_win", run_options.brains_to_win);
      run_options.max_turns = IniParser::get_config<int>("Game.max_turns", run_options.max_turns);

      run_options.weak_die_faces
        = IniParser::get_config<std::string>("Dice.weak_die_faces", run_options.weak_die_faces);
      run_options.strong_die_faces
        = IniParser::get_config<std::string>("Dice.strong_die_faces", run_options.strong_die_faces);
      run_options.tough_die_faces
        = IniParser::get_config<std::string>("Dice.tough_die_faces", run_options.tough_die_faces);
    } else {
      std::cout << "--> using default config\n";
    }
  }

  setup(run_options);
}
