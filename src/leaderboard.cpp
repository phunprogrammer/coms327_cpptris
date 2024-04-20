#include <leaderboard.h>
#include <fstream>
#include <sstream>
#include <iostream>

Leaderboard::Leaderboard() {
    std::ifstream board("./data/leaderboard.csv");

    if(!board.is_open()) return;

    std::string line;
    while (std::getline(board, line)) {
        std::stringstream stream(line);

        std::string name;
        int score;
        std::string temp;

        std::getline(stream, name, ',');
        std::getline(stream, temp);
        score = atoi(temp.c_str());
        entries.insert(Entry<std::string>(name, score));
    }

    board.close();
}