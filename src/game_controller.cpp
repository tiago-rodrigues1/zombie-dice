#include "game_controller.hpp"

#include <random>

std::vector<std::string> DRA::roll_dices() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<std::string> chosen_faces;
  std::vector<Dice> remaining_dices;

  for (size_t i{ 0 }; i < chosen_dices.size(); ++i) {
    std::uniform_int_distribution<> dist(0, 5);
    auto index = dist(gen);

    std::string face(1, chosen_dices[i].faces[index]);

    // removemos aqueles que são brains e gun
    if (face == "s") {
      remaining_dices.push_back(chosen_dices[i]);
    }

    chosen_faces.push_back(face);
  }

  chosen_dices = std::move(remaining_dices);

  return chosen_faces;
}

void DiceBag::get_dices(Player& player) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<size_t> weights;

  for (const auto& pair : dice_bag) {
    size_t count = pair.second;
    weights.push_back(count);
  }

  for (int i{ 0 }; i < 3; ++i) {

    std::discrete_distribution<> dist(weights.cbegin(), weights.cend());
    auto index = dist(gen);

    auto it = dice_bag.begin();
    std::advance(it, index);
    player.player_DRA.chosen_dices.push_back(it->first);
  }
}

void GameController::parse_config(int argc, char* argv[]) {
  // alguma coisa
}