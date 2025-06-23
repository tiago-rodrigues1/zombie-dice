#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <cstdint>
#include "game_controller.hpp"

class Player {
public:
  std::string name;
  size_t points{ 0 };
  size_t count_turns{0};
private:
};

#endif