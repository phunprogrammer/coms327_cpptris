#include <game.h>
#include <ncurses.h>

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
        std::map<blockEnum, int> count;

        WINDOW* menuWin;
        WINDOW* gameWin;
        WINDOW* lineWin;
        WINDOW* scoreWin;
        WINDOW* nextWin;
        WINDOW* levelWin;
    public:
        GameManager();
        ~GameManager();
        
        void InitColors();
        int StartGame(int level);

        int PrintBoard();
        int PrintLines();
        int PrintScores();
        int PrintNext();
        int PrintLevel();

        int SelectLevel();

        int UpdateScreen();
        int HandleInput(int inputs);
        int GetSpeed();
        int IncrementLines(int lines);
        int IncrementCount(blockEnum block);
};

#endif