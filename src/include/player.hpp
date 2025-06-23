#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <cstdint>

class Player {
public:
  std::string name;
  size_t points{ 0 };

  Player() {};
private:
};

#endif