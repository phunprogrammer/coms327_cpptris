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

Leaderboard::~Leaderboard() {}

int Leaderboard::insert(std::string name, int score) {
    if(entries.size() < LEADERBOARD_SIZE){
        entries.insert(Entry<std::string>(name, score));
        return 1;
    }
    else if (score > (*--entries.end()).getPriority()) {
        entries.erase(--entries.end());
        entries.insert(Entry<std::string>(name, score));
        return 1;
    }
    else 
        return 0;
}

void Leaderboard::Print() {
    for(auto& entry : entries)
        std::cout << entry.getData() << "," << entry.getPriority() << std::endl;
}