#include <gamemanager.h>
#include <iostream>
#include <chrono>
#include <thread>

GameManager::GameManager() : score(0), time(0) {}

int GameManager::StartGame() {
    game = Game();

    while(1) {
        PrintBoard();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(!game.Drop() && !game.SpawnBlock())
            break;
    }

    return 1;
}

int GameManager::PrintBoard() {
    for(int y = 0; y < BOARD_ROWS; y++) {
        for(int x = 0; x < BOARD_COLS; x++)
            std::cout << game.getBoard()[y][x];

        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 1;
}

int GameManager::HandleInput(char input) {
    return 1;
}