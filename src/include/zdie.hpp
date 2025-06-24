#ifndef ZDIE_HPP
#define ZDIE_HPP

#include <cstdint>
#include <string>

enum dice_type_e : std::uint8_t {
  GREEN = 1,
  ORANGE,
  RED,
};

class Zdie {
private:
std::string faces;
public:
  dice_type_e type;
  Zdie(dice_type_e t, std::string f);
  char roll();
};


#endif