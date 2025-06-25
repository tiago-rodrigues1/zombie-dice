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

  last_rolled_face = face_map[face];

  return last_rolled_face;
}

std::string Zdie::face_to_emoji() const { 
  std::unordered_map<ZdieFaces, std::string> face_map {
    { ZdieFaces::BRAIN, "🧠" },
    { ZdieFaces::SHOTGUN, "💥" },
    { ZdieFaces::FOOTPRINT, "👣" },
  };

  return face_map[last_rolled_face];
}

std::string Zdie::type_to_emoji() const { 
  std::unordered_map<dice_type_e, std::string> type_map {
    { dice_type_e::GREEN, "🟩" },
    { dice_type_e::ORANGE, "🟧" },
    { dice_type_e::RED, "🟥" },
  };

  return type_map[type];
}
