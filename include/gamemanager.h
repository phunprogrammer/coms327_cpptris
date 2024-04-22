#include <game.h>
#include <leaderboard.h>
#include <ncurses.h>
#include <chrono>
#include <string>

#define COLOR_ORANGE 8
#define FRAMES 60.0
#define MILLIS_PER_FRAME (1000.0 / FRAMES)
#define DROP_PAUSE 250

#define BORDER 1
#define LINES_HEIGHT 4
#define SCORE_HEIGHT 4
#define NEXT_HEIGHT 7
#define LEVELS_HEIGHT 4

const std::string AUDIO_PATH = "./data/audio/"; 

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
        Leaderboard board;

        WINDOW* menuWin;
        WINDOW* boardWin;
        WINDOW* endWin;

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
        int PrintLeaderBoard();
        int PrintEnd();

        int InitTimeWin();
        int PrintTime();
        int InitGame(int level);

        int SelectLevel();
        int PrintLogo();

        int UpdateScreen();
        int HandleInput(int inputs, double& accumulator);
        int GetSpeed();
        int IncrementLines(int lines);
        int IncrementCount(blockEnum block);
};

#endif