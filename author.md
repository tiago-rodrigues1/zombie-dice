# Zombie Dice

This project is a C++ implementation of the Zombie Dice game, a fast-paced dice-rolling game where players take on the role of zombies competing to eat the most brains. The game features:

- Support for multiple players
- Configurable settings via .ini files
- A turn-based system with dice of varying difficulty (weak, strong, tough)
- Text-based user interface with organized output areas (scoreboard, actions, messages)
- State machine for handling game flow and logic
  
# Author(s)

Tiago Rodrigues dos Santos
tiago.rodrigues.709@ufrn.edu.br 

Luisa Ferreira de Souza Santos
luisaferreirass08@gmail.com

# Compiling and Running

cmake -S . -B build

cmake --build build

./build/zdice (.ini file path)
