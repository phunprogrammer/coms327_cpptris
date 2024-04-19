#include <game.h>

Game::Game() : board(BOARD_ROWS, std::vector<int>(BOARD_COLS, 0)), score(0), time(0) {}