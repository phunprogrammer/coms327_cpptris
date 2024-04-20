#include <game.h>
#include <ncurses.h>
#include <chrono>

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager {
    private:
        Game game;
        int lines;
        int score;
        int level;
        std::chrono::_V2::system_clock::time_point time;
        std::map<blockEnum, int> count;

        WINDOW* menuWin;
        WINDOW* gameWin;
        WINDOW* lineWin;
        WINDOW* scoreWin;
        WINDOW* nextWin;
        WINDOW* levelWin;
        WINDOW* countWin;
        WINDOW* timeWin;
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
        int PrintCount();

        int InitTimeWin();
        int PrintTime();
        int InitGame(int level);

        int SelectLevel();
        int PrintLogo();

        int UpdateScreen();
        int HandleInput(int inputs);
        int GetSpeed();
        int IncrementLines(int lines);
        int IncrementCount(blockEnum block);
};

#endif