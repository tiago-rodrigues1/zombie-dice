#include <iomanip>
#include <iostream>

#include "views.hpp"

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
  
  std::cout << "\nMinimum of " << min_players << " player(s), maximum of " 
            << max_players << " names."
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


void Views::title_and_message_area() {
  std::cout << std::setw(20) << "→☣️ [🧟] Zombie Dice Delux, v 0.1 [🧟] ☣️←" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "┌────────────────────────┐" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "│      Global Score      │" << std::setw(20) << "\n";
  std::cout << std::setw(20) << "└────────────────────────┘" << std::setw(20) << "\n";

  // terminar
}
