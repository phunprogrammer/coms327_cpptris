#include <gamemanager.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <ncurses.h>
#include <queue>

#define COLOR_ORANGE 8
#define FRAMES 60

GameManager::GameManager() : lines(0), score(0), time(0) {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    start_color();
    InitColors();

    if(can_change_color()) {
        init_color(COLOR_ORANGE, 1000, 500, 0);
        init_pair(blockEnum::L, COLOR_ORANGE, COLOR_BLACK);
    }
}

GameManager::~GameManager() {
    endwin();
}

void GameManager::InitColors() {
    init_pair(blockEnum::I, COLOR_CYAN, COLOR_BLACK);
    init_pair(blockEnum::O, COLOR_YELLOW, COLOR_BLACK);
    init_pair(blockEnum::L, COLOR_WHITE, COLOR_BLACK);
    init_pair(blockEnum::J, COLOR_BLUE, COLOR_BLACK);
    init_pair(blockEnum::S, COLOR_GREEN, COLOR_BLACK);
    init_pair(blockEnum::Z, COLOR_RED, COLOR_BLACK);
    init_pair(blockEnum::T, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(BLOCKS.size() + 1, COLOR_BLACK, COLOR_BLACK);
}

int GameManager::StartGame(int level) {
    game = Game();
    PrintBoard();
    startLevel = this->level = level;

    std::queue<int> inputs;
    auto startTime = std::chrono::steady_clock::now();

    while(1) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        int input = HandleInput(getch());

        if(input == 2) break;
        else if(input) flushinp();

        if (elapsedTime >= GetSpeed()) {
            startTime = currentTime;

            if(!IncrementLines(game.Drop()))
                break;

            PrintBoard();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FRAMES));
    }

    return 1;
}

int GameManager::PrintBoard() {
    for(int y = BOARD_SPAWN_Y; y < BOARD_ROWS; y++)
        for(int x = 0; x < BOARD_COLS; x++) {
            int color;

            if(!game.getBoard()[y][x])
                color = BLOCKS.size() + 1;
            else
                color = game.getBoard()[y][x];

            attron(COLOR_PAIR(color));
            mvprintw(y, x * 2, "██");
            attroff(COLOR_PAIR(color));
        }

    refresh();

    return 1;
}

int GameManager::HandleInput(int input) {
    int out;

    switch(input) {
        case 'z':
            out = game.RotateCCW();
            break;
        case 'x':
            out = game.RotateCW();
            break;
        case KEY_LEFT:
            out = game.MoveLeft();
            break;
        case KEY_RIGHT:
            out = game.MoveRight();
            break;
        case KEY_DOWN:
            if(!IncrementLines(game.Drop()))
                return 2;
            break;
        default:
            return 0;
    }

    if(out)
        PrintBoard();

    return 1;
}

int GameManager::GetSpeed() {
    float speed;

    if(level == 9)
        speed = 6;
    else if(level >= 10 && level <= 12)
        speed = 5;
    else if(level >= 13 && level <= 15)
        speed = 4;
    else if(level >= 16 && level <= 18)
        speed = 3;
    else if(level >= 19 && level <= 28)
        speed = 2;
    else if(level >= 29)
        speed = 1;
    else
        speed = (FRAMES * 0.75) - level * (FRAMES / 12.0);

    return 1000 * (speed / FRAMES);
}

int GameManager::IncrementLines(int lines) {
    if(!lines) return 0;
    this->lines += lines - 1;

    if(this->lines >= (level * 10 + 10))
        level++;

    switch(lines) {
        case 2:
            score += 40 * (level + 1);
            break;
        case 3:
            score += 100 * (level + 1);
            break;
        case 4:
            score += 300 * (level + 1);
            break;
        case 5:
            score += 1200 * (level + 1);
            break;
        default:
            break;
    }

    return lines;
}