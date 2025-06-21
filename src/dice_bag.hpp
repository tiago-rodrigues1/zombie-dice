#ifndef DICE_BAG_HPP
#define DICE_BAG_HPP

#include <cstddef>
#include <vector>

#include "zdie.hpp"

class DiceBag {
private:
  std::vector<Zdie> dices;
public:
  DiceBag() {};

  size_t count_dices();
  void shuffle_bag();
  std::vector<Zdie> draw(int n = 3);
  void add_dices(std::vector<Zdie> new_dices);
};
#endif