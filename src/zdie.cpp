#include <random>

#include "zdie.hpp"

Zdie::Zdie(dice_type_e t, std::string f) : type(t), faces(f) {}

char Zdie::roll() {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> distrib(0, (faces.size() - 1));
  auto idx = distrib(gen);

  return faces[idx];
}
