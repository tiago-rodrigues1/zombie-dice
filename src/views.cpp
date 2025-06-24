#include <algorithm>
#include <iomanip>
#include <iostream>

#include "include/views.hpp"

void Views::welcome_message(size_t min_players, size_t max_players) {
  std::cout << R"(         ---> Welcome to the Zombi Dice game (v 0.1) <--

-- copyright DIMAp/UFRN 2024-2025 --

The object of the jeopardy dice game Zombie Dice is to be the
first to eat 13 or more human brains in a turn.
Each player's turn consists of repeatedly rolling 3 dice.
A die may produce 3 outcomes: you ate a brain, the human escaped,
or you were shot!

If you get shot 3 or more times you die and lose all brains you
ate in that turn; then the turn goes to the next player.
However, if you decide to stop before getting shot 3 times, you
keep the brains you ate.

Before we begin, please enter the names of the players
in a single line, separated by comma. For example:
>>> Jack, Menace, Ultraz, Boris
)";

  std::cout << "\nMinimum of " << min_players << " player(s), maximum of " << max_players
            << " names."
            << "\nProviding an empty line will end the game."
            << "\n\n>>>\n";
}

void Views::show_players_message(const std::vector<Player>& players) {
  std::cout << ">>> The players of the game are:\n";
  for (Player player : players) {
    std::cout << "\"" << player.name << "\"\n";
  }

  std::cout << ">>> The player who will start the game is \"" << players[0].name << "\"\n";
  "Press <Enter> to start the match.\n";
}

void print_points(const Player& player) {
  for (size_t i{ 0 }; i < 13; ++i) {
    if (i < player.points) {
      std::cout << "🧠 ";
    } else {
      std::cout << "🔶 ";
    }
  }
}

void Views::title_and_scoreboard_area(const std::vector<Player>& players,
                                      size_t current_player_index) {
  std::cout << std::setw(20) << "→☣️ [🧟] Zombie Dice Delux, v 0.1 [🧟] ☣️←" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "┌────────────────────────┐" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "│      Global Score      │" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "└────────────────────────┘" << std::setw(20) << "\n";

  for (size_t i{ 0 }; i < players.size(); ++i) {

    if (i == current_player_index) {
      std::cout << "> ";
    } else {
      std::cout << "  ";
    }
    std::cout << std::left << std::setw(12) << players[i].name << ": ";
    print_points(players[i]);
    std::cout << "| (" << players[i].points << "), # turns played: " << players[i].count_turns
              << "\n";
  }
}

void Views::area_points(const std::vector<Zdie>& points) {
  for (size_t i{ 0 }; i < points.size(); ++i) {
    if (points[i].type == dice_type_e::GREEN) {
      std::cout << "🟩 ";
    } else if (points[i].type == dice_type_e::ORANGE) {
      std::cout << "🟧 ";
    } else if (points[i].type == dice_type_e::RED) {
      std::cout << "🟥 ";
    }
  }
  std::cout << "\n";
}

void Views::areas(Player player,
                  size_t quantity_dices,
                  std::vector<Zdie> BSA,
                  std::vector<Zdie> SSA) {
  std::cout << "\"" << player.name << "\" | Turn #: " << player.count_turns
            << " | Bag has: " << quantity_dices << " 🎲.\n\n";

  std::cout << "🧠: ";
  area_points(BSA);
  std::cout << "💥: " << "\n";
  area_points(SSA);
  std::cout << "\n";
}

void Views::rolling_table(std::vector<std::pair<ZdieFaces, dice_type_e>> faces) {
  std::string title{ "Rolling Table" };
  size_t box_width{ 40 };
  size_t padding_title{ (box_width - title.size()) / 2 };
  size_t cols = 3;
  size_t col_width = box_width / cols;
  std::cout << "┌" << std::string(box_width, '─') << "┐\n";

  std::cout << "│" << std::string(padding_title, ' ') << title
            << std::string(box_width - padding_title - title.size(), ' ') << "│\n";

  std::cout << "├";
  for (int i = 0; i < 3; ++i) {
    for (size_t j{ 0 }; j < 13; ++j) {
      std::cout << '─';
    }
    if (i < 2) {
      std::cout << "┬";
    } else {
      std::cout << "┤\n";
    }
  }
  std::cout << "│";
  for (size_t i{ 0 }; i < 3; ++i) {
    std::string line{ "" };
    switch (faces[i].first) {
    case ZdieFaces::BRAIN:
      line += "🧠";
      break;
    case ZdieFaces::SHOTGUN:
      line += "💥";
      break;
    case ZdieFaces::FOOTPRINT:
      line += "👣";
      break;
    default:
      break;
    }

    switch (faces[i].second) {
    case dice_type_e::GREEN:
      line += "(🟩)";
      break;
    case dice_type_e::ORANGE:
      line += "(🟧)";
      break;
    case dice_type_e::RED:
      line += "(🟥)";
      break;
    default:
      break;
    }

    size_t cell_width = 13;
    size_t content_width = 6;  //
    size_t padding = (cell_width - content_width) / 2;
    std::cout << std::string(padding, ' ') << line
              << std::string(cell_width - padding - content_width, ' ');

    std::cout << "│";
  }
  std::cout << "\n└────────────────────────────────────────┘" << "\n";
}

void Views::message_area(std::vector<std::string> messages) {
  size_t box_width{ 40 };
  size_t padding_left{ 3 };

  
  std::cout << "┌─[ Message area ]";
  for (size_t i{ 0 }; i < box_width - 17; ++i) {
    std::cout << "─";
  }
  std::cout << "┐\n";

  for (std::string message : messages) {
    if (message.size() + padding_left > box_width) {
      message = message.substr(0, box_width - padding_left - 1);  // corta a mensagem para caber
    }
      size_t padding_right{ box_width - message.size() - padding_left };
      std::cout << "│" << std::string(padding_left, ' ') << message
                << std::string(padding_right, ' ') << "│\n";
    
  }

  std::cout << "└";
  for (size_t i{ 0 }; i < box_width; ++i) {
    std::cout << "─";
  }
  std::cout << "┘";
}
