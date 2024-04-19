#include <gamemanager.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <ncurses.h>

GameManager::GameManager() : score(0), time(0) {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    start_color();
    InitColors();
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

int GameManager::StartGame() {
    game = Game();
    PrintBoard();

    auto startTime = std::chrono::steady_clock::now();

    while(1) {
        if(HandleInput(getch()) == 2)
            break;

        flushinp();

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        if (elapsedTime >= 50) {
            startTime = currentTime;

            if(!game.Drop())
                break;

            PrintBoard();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
            if(!game.Drop())
                return 2;
            break;
        default:
            return 0;
    }

    if(out)
        PrintBoard();

    return 1;
}