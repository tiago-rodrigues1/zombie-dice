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


enum GameState : std::uint8_t { WELCOME, READ_PLAYERS, DEFINE_FIRST_PLAYER, READ_ACTION, END_TURN };

class DRA {
public:
  std::vector<Zdie> chosen_dices;
};


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
  static DRA dice_area;
  void parse_config(int argc, char* argv[]);
  void process_events();
  void update();
  void render();
};

#endif