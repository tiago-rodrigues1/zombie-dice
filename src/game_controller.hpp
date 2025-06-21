#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

enum dice_colors_e : std::uint8_t {
  GREEN = 1,
  ORANGE,
  RED,
};

enum actions_e : std::uint8_t { HOLD, ROLL, QUIT_TURN };

enum GameState : std::uint8_t { WELCOME, READ_PLAYERS, DEFINE_FIRST_PLAYER, READ_ACTION, END_TURN };

class Dice {
private:
  dice_colors_e color;

public:
  std::string faces;
  Dice(dice_colors_e color) : color(color) {
    if (color == dice_colors_e::GREEN) {
      faces = "bbbffs";
    } else if (color == dice_colors_e::ORANGE) {
      faces = "bbffss";
    } else if (color == dice_colors_e::RED) {
      faces = "bffsss";
    }
  }
};

class DRA {
public:
  std::vector<Dice> chosen_dices;
  std::vector<std::string> roll_dices();
};

class Player {
public:
  std::string name;
  size_t points{ 0 };
  DRA player_DRA;
  actions_e current_action;
  void apply_action();

private:
};

// class DiceBag {
// public:
// std::map<Dice, size_t> dice_bag = { { Dice(GREEN), 6 }, { Dice(ORANGE), 4 }, { Dice(RED), 3 } };
//  pensei em deixarmos que nem estão os dados "gggggoooorrr"...;

// size_t number_of_dices{13};

// void get_dices(Player& player);
//}

class GameController {
private:
  GameState game_state;
  std::vector<Player> players;
  Player current_player;

  void welcome_message();
  std::vector<std::string> read_players();
  void define_players(std::vector<std::string> players_name);
  void define_first_player();
  void players_message();
  void read_actions();

public:
  void parse_config(int argc, char* argv[]);
  void process_events();
  void update_events();
  void render();
};

#endif