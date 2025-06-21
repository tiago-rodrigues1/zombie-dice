#include "player.hpp"



void Player::apply_action(){
  switch (current_action)
  {
  case actions_e::ROLL:
    GameController::dice_area.chosen_dices = DiceBag::draw();
    break;
  
  default:
    break;
  }
}