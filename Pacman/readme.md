Pacman
=========================
By Kirill Poligach

Arcade real-time game, the goal is to eat all the tiles in the maze.
The ghosts are trying to kill the pacman. When the pacman eats all the ticks,
the level and difficulty of the game changes.

The SDL2 library was used in the implementation.

Options:

1: Standart game, with classic rules from the original game ( New game ).

2: Saving a game to a file ( F5 key ).

3: Load the game from the file ( Load game ).

4: Load custom game with custom rules from the file ( Custom game ).

5: Pause ( spacebar key ).

6: Exit ( escape key ).

7: You can move the packman with the arrows.

8: The game automatically saves the highscore.

Files (saveFile and customFile) Contain map, the map is presented as:

: 'G' - closed gates

: 'g' - opened gates

: 'T' - tunnel

: 'r' - road without points

: 'W' - wall

: 'o' - small point ( 10 score points )

: 'O' - energizer ( 50 score points ), changes game mode

: 'F' - fruit ( the amount of score points depends on the game level )

File highScore contains game's high score.
