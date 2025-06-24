#ifndef ZDIE_HPP
#define ZDIE_HPP

#include <cstdint>
#include <string>

enum ZdieFaces {
  BRAIN,
  SHOTGUN,
  FOOTPRINT
};

enum dice_type_e : std::uint8_t {
  GREEN = 1,
  ORANGE,
  RED,
};

class Zdie {
private:
  dice_type_e type;
  std::string faces;
public:
  Zdie(dice_type_e t, std::string f);
  ZdieFaces roll();
};


#endif