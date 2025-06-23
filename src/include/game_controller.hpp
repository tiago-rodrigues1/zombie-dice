#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "dice_bag.hpp"
#include "player.hpp"
#include "zdie.hpp"

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
  END_TURN,
  QUIT,
  HOLD
};

inline std::string to_string(GameState state) {
  switch (state) {
    case NEUTRAL:      return "NEUTRAL";
    case START:        return "START";
    case WELCOME:      return "WELCOME";
    case READ_PLAYERS: return "READ_PLAYERS";
    case READ_ACTION:  return "READ_ACTION";
    case ROLL:         return "ROLL";
    case END_TURN:     return "END_TURN";
    case QUIT:         return "QUIT";
    case HOLD:         return "HOLD";
    default:           return "UNKNOWN";
  }
}

class GameController {
private:
  GameState game_state;
  std::vector<Player> players;
  std::vector<Zdie> DRA;
  std::vector<Zdie> BSA;
  std::vector<Zdie> SSA;
  DiceBag dice_bag;
  size_t current_player_idx;
  size_t min_players;
  size_t max_players;

  std::vector<std::string> read_players();
  void define_players(std::vector<std::string> players_names);
  void read_actions();
  void roll_dices();
  void draw_dices();
  void update_player();
  void points_to_player();

  void setup(const RunningOpt& run_options);

public:
  GameController() {
    min_players = 2;
    max_players = 6;
    game_state = NEUTRAL;
  }

  void print_state();

  void parse_config(int argc, char* argv[]);
  void process_events();
  void update();
  void render();
  bool game_over(bool quit_game = false);
};

#endif