#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <cstdint>
#include "game_controller.hpp"
enum actions_e : std::uint8_t { HOLD, ROLL, QUIT_TURN };

class Player {
public:
  std::string name;
  size_t points{ 0 };;
  actions_e current_action;
  void apply_action();

private:
};

#endif