#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "zdie.hpp"
#include "player.hpp"
#include "dice_bag.hpp"


enum GameState : std::uint8_t { WELCOME, READ_PLAYERS, READ_ACTION, ROLL, END_TURN };

class DRA {
public:
  std::vector<Zdie> chosen_dices;
};


class GameController {
  private:
  GameState game_state;
  std::vector<Player> players;
  std::vector<Zdie> DRA;
  std::vector<Zdie> BSA;
  std::vector<Zdie> SSA;
  DiceBag& dice_bag;
  size_t players_count;
  
  void welcome_message();
  std::vector<std::string> read_players();
  void define_players(std::vector<std::string> players_name);
  void define_first_player();
  void players_message();
  void read_actions();
  void roll_dices();
  void draw_dices();
  void update_player();
  void points_to_player();
  bool checks_if_all_player_had_the_same_turns();
  
  public:
  GameController();
  void parse_config(int argc, char* argv[]);
  void process_events();
  void update();
  void render();
  bool game_over(bool quit_game = false);
};

#endif