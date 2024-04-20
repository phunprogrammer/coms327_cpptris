#include <set>
#include <string>

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define LEADERBOARD_SIZE 8

template<typename T>
class Entry {
    private:
        T data;
        int priority;

    public:
        Entry(T data, int priority) : data(data), priority(priority) {}

        T getData() const { return data; }

        int getPriority() const { return priority; }

        bool operator < (const Entry &item) const { return getPriority() > item.getPriority(); }
};

class Leaderboard {
    private:
        std::multiset<Entry<std::string>> entries;
    public:
        Leaderboard();
        ~Leaderboard();
        void Print();

        int Insert(std::string name, int score);
        int Save();
        const std::multiset<Entry<std::string>>& getEntries() const { return entries; }
};

#endif