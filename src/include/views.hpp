#ifndef VIEWS_HPP
#define VIEWS_HPP

#include <string>
#include <vector>
#include "zdie.hpp"

#include "player.hpp"

class Views {
private:
public:
Views() {};
  static void welcome_message(size_t min_players, size_t max_players);
  static void show_players_message(const std::vector<Player>& players);
  static void title_and_scoreboard_area(const std::vector<Player>& players, size_t current_player_index);
  static void area_points(const std::vector<Zdie>& points);
  static void areas(Player player, size_t quantity_dices);
  static void message_area(std::vector<std::string> messages);
  static void usage();
  static void rolling_table(const std::vector<Zdie>& dices, const std::vector<Zdie>& BSA, const std::vector<Zdie>& SSA);
};

#endif