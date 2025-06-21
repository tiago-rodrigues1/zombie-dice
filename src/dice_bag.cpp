#include <algorithm>    // std::shuffle
#include <chrono>       // std::chrono::system_clock
#include <random>       // std::default_random_engine

#include "dice_bag.hpp"

size_t DiceBag::count_dices() { return dices.size(); }

void DiceBag::shuffle_bag() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle (dices.begin(), dices.end(), std::default_random_engine(seed));
}

std::vector<Zdie> DiceBag::draw(int n) {
  shuffle_bag();
  std::vector<Zdie> drawed_dices(dices.end() - n, dices.end());
  dices.erase(dices.end() - n,  dices.end());

  return drawed_dices;
}

void DiceBag::add_dices(std::vector<Zdie> new_dices) {
  dices.insert(dices.end(), new_dices.begin(), new_dices.end());
}
