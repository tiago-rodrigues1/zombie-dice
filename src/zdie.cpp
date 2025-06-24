#include <random>
#include <unordered_map>

#include "include/zdie.hpp"

Zdie::Zdie(dice_type_e t, std::string f) : type(t), faces(f) {}

ZdieFaces Zdie::roll() {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> distrib(0, (faces.size() - 1));
  auto idx = distrib(gen);

  char face = faces[idx];

  std::unordered_map<char, ZdieFaces> face_map{
    { 'b', ZdieFaces::BRAIN },
    { 's', ZdieFaces::SHOTGUN },
    { 'f', ZdieFaces::FOOTPRINT },
  };

  return face_map[face];
}
