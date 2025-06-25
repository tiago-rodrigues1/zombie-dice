#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "dice_bag.hpp"
#include "player.hpp"
#include "zdie.hpp"
#include "views.hpp"

struct RunningOpt {
  int weak_dice{ 6 };
  int strong_dice{ 4 };
  int tough_dice{ 3 };
  int max_players{ 6 };
  int brains_to_win{ 3 };
  int max_turns{ 0 };
  std::string weak_die_faces{ "bbbffs" };
  std::string strong_die_faces{ "bbffss" };
  std::string tough_die_faces{ "bffsss" };
};

enum GameState : std::uint8_t {
  NEUTRAL,
  START,
  WELCOME,
  READ_PLAYERS,
  READ_ACTION,
  ROLL,
  LOSE_TURN,
  RECYCLE,
  END_TURN,
  QUIT,
  HOLD,
  TIE_BREAKER,
  END_GAME
};

inline std::string to_string(GameState state) {
  switch (state) {
    case RECYCLE:      return "RECYCLE";
    case NEUTRAL:      return "NEUTRAL";
    case START:        return "START";
    case WELCOME:      return "WELCOME";
    case READ_PLAYERS: return "READ_PLAYERS";
    case READ_ACTION:  return "READ_ACTION";
    case ROLL:         return "ROLL";
    case LOSE_TURN:    return "LOSE_TURN";
    case END_TURN:     return "END_TURN";
    case QUIT:         return "QUIT";
    case HOLD:         return "HOLD";
    case END_GAME:     return "END_GAME";
    default:           return "UNKNOWN";
  }
}

inline void print_running_opt(const RunningOpt& opt) {
    std::cout << "RunningOpt configuration:\n";
    std::cout << "  weak_dice      : " << opt.weak_dice << '\n';
    std::cout << "  strong_dice    : " << opt.strong_dice << '\n';
    std::cout << "  tough_dice     : " << opt.tough_dice << '\n';
    std::cout << "  max_players    : " << opt.max_players << '\n';
    std::cout << "  brains_to_win  : " << opt.brains_to_win << '\n';
    std::cout << "  max_turns      : " << opt.max_turns << '\n';
    std::cout << "  weak_die_faces : " << opt.weak_die_faces << '\n';
    std::cout << "  strong_die_faces: " << opt.strong_die_faces << '\n';
    std::cout << "  tough_die_faces : " << opt.tough_die_faces << '\n';
}

class GameController {
private:
  GameState game_state;
  std::vector<Player> players;
  std::vector<Zdie> DRA;
  std::vector<Zdie> BSA;
  std::vector<Zdie> SSA;
  std::vector<Zdie> prev_dra;
  std::map<std::string, size_t> partial_points;
  DiceBag dice_bag;
  size_t current_player_idx;
  size_t min_players;
  size_t max_players;
  Player winner;
  size_t limit_of_turns{0};
  size_t highest_point{4};
  Views view;
  
  std::vector<std::string> read_players();
  void define_players(std::vector<std::string> players_names);
  void read_actions();
  void roll_dices();
  void handle_roll();
  void handle_hold();
  void handle_quit();
  void dices_back_to_bag();
  void end_turn();
  void recycle();
  void next_player();
  void consolidate_points();
  bool all_turns_completed();
  bool player_can_play();
  void checks_end_of_game();
  std::vector<Player> get_highest_players();
  
  void setup(const RunningOpt& run_options);
  
  public:
  GameController() : view() { // Initialize view with default constructor
    min_players = 2;
    max_players = 6;
    game_state = NEUTRAL;
  }

  void print_state();

  void parse_config(int argc, char* argv[]);
  void process_events();
  void update();
  void render();
  bool game_over();
};

#endif