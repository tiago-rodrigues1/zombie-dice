#ifndef VIEWS_HPP
#define VIEWS_HPP

#include <string>
#include <vector>

#include "player.hpp"

class Views {
private:
  Views() {};
public:
  static void welcome_message(size_t min_players, size_t max_players);
  static void show_players_message(const std::vector<Player>& players);
  static void title_and_message_area();
};

#endif