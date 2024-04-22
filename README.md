# COMS 327 - Assignment 10
This is assignment 10 for COMS 327. This is CPPTris, a NES Tetris game implementation using C++ and ncurses. Most functions of the game are true to the original game (or as close as I could make it). The only mode that is implemented is marathon mode with a level selector. Also, there is a locally stored leaderboard that shows the top 8 scores on the machine. 
NOTICE: There is a version of this game that has audio, unfortunately, the pyrite server does not support SDL2-devel library (I thought it did), so this a version with no sounds. Also use `BACKSPACE` to exit the game instead of `CTRL+C`.

## Controls
- **`Arrow keys`**: Move Tetrimino and navigate menu.
- **`Z, X`**: Rotate counter clockwise and clockwise respectively.
- **`Enter`**: Interact with menus and pause the game.
- **`Backspace`**: Exit game.

## HOW TO RUN:
To run use `make` in the terminal then run the `game` executable generated from it. Use `make clean` to delete generated files.