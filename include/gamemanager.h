#include <game.h>

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager {
    private:
        Game game;
        int score;
        float time;
    public:
        GameManager();
        int StartGame();
        int PrintBoard();
        int HandleInput(char input);

        Game& getGame() { return game; }
};

#endif