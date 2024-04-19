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
        ~GameManager();
        
        void InitColors();
        int StartGame();
        int PrintBoard();
        int HandleInput(int input);
};

#endif