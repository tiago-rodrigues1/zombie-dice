#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>
#include <string>

enum dice_colors_e : std::uint8_t {
  GREEN = 1,
  ORANGE,
  RED,
};

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

class Player {
  public:
    std::string name;
    size_t points;
    DRA player_DRA;
};

class DRA {
  public:
    std::vector<Dice> chosen_dices;
    std::vector<std::string> roll_dices();
};

class DiceBag {
public:
  std::map<Dice, size_t> dice_bag = { { Dice(GREEN), 6 }, { Dice(ORANGE), 4 }, { Dice(RED), 3 } };
  // pensei em deixarmos que nem estão os dados "gggggoooorrr"...;

  size_t number_of_dices{13};

  void get_dices(Player& player);
};

class GameController{

  public:
  void parse_config(int argc, char* argv[]);

};

#endif