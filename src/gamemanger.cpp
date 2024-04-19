#include <gamemanager.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <ncurses.h>

GameManager::GameManager() : score(0), time(0) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
}

int GameManager::StartGame() {
    game = Game();
    PrintBoard();

    auto startTime = std::chrono::steady_clock::now();

    while(1) {
        HandleInput(getch());
        flushinp();

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        if (elapsedTime >= 1000) {
            startTime = currentTime;

            if(!game.Drop() && !game.SpawnBlock())
                break;

            PrintBoard();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 1;
}

int GameManager::PrintBoard() {
    for(int y = BOARD_SPAWN_Y; y < BOARD_ROWS; y++)
        for(int x = 0; x < BOARD_COLS; x++)
            mvprintw(y, x, "%d", game.getBoard()[y][x]);

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
            out = game.Drop();
            break;
        default:
            return 0;
    }

    if(out)
        PrintBoard();

    return 1;
}