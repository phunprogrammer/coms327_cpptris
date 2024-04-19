#include <game.h>

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager {
    private:
        Game game;
        int lines;
        int score;
        int startLevel;
        int level;
        float time;
    public:
        GameManager();
        ~GameManager();
        
        void InitColors();
        int StartGame(int level);
        int PrintBoard();
        int HandleInput(int inputs);
        int GetSpeed();
        int IncrementLines(int lines);
};

#endif