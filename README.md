# Programming Assignment: Zombie Dice Project

This project implements a terminal-based version of **Zombie Dice**. The game uses Object-Oriented Modeling, random number generation, separate compiling, progressive programming, and data structures. The architecture is based on the [game-loop pattern](https://gameprogrammingpatterns.com/game-loop.html) and includes an interactive terminal interface.

## Gameplay

Players are zombies (🧟) competing to collect brains and avoid shotguns. The player with the most brains at the end wins. The game proceeds in turns, with each player rolling dice representing humans (green, yellow, red) and sorting outcomes into brains, shotguns, or footprints. Turns end when a player is shot three times or voluntarily stops to keep brains collected that round.

The game handles all rules of Zombie Dice, including:
- Rolling and sorting dice
- Turn management and handover
- Storing and returning dice to the bag
- Tie-breaking rounds
- Optional maximum turns mode via config
- Scoring and winner determination

## Configuration

Settings are read from an INI-format config file (e.g., `zdice.ini`):

```ini
[Game]
weak_dice = 6
strong_dice = 4
tough_dice = 3
max_players = 6
brains_to_win = 13
max_turns = 0 # Value 0 means the game plays without limit of turns.

[Dice]
weak_die_faces   = "bbbffs"
strong_die_faces = "bbffss"
tough_die_faces  = "bffsss"
```

Run the game with:

```bash
./zdice zdice.ini
```

If no config file is provided, defaults are used.

## Interface

On launch, the game displays a welcome screen and prompts for player names. The play screen shows:
- Title
- Scoreboard
- Dice rolling area (DRA), brain storage area (BSA), shotgun storage area (SSA)
- Message area
- Player command prompt

Screenshots and interface details are provided in the documentation.

## System Modeling

Entities modeled include:
- `ZDie`: Represents dice.
- `Player`: Tracks player name and brains collected.
- `DiceBag`: Stores and manages dice.
- `GameController`: Manages game state, turns, scores, and flow.

## Output

The game prints screens reflecting the current state, scores, and prompts for player actions.
