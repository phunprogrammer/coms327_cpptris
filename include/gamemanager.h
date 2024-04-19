#include "game.h"

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager {
    private:
        Game current;

    public:
        GameManager();
        int StartGame();
        int PrintBoard();
        int HandleInput(char input);
};

#endif