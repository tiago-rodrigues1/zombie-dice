#include "include/common.hpp"
#include "include/dice_bag.hpp"

size_t DiceBag::count_dices() { return dices.size(); }

void DiceBag::shuffle_bag() {
  shuffler(dices.begin(), dices.end());
}

std::vector<Zdie> DiceBag::draw(int n) {
  shuffle_bag();
  std::vector<Zdie> drawed_dices(dices.end() - n, dices.end());
  dices.erase(dices.end() - n,  dices.end());

  return drawed_dices;
}

void DiceBag::add_dices(Zdie dice, size_t n) {
  for (size_t i{ 0 }; i < n; ++i) {
    dices.emplace_back(dice);
  }
}

void DiceBag::add_dices(std::vector<Zdie> new_dices) {
  dices.insert(dices.end(), new_dices.begin(), new_dices.end());
}
