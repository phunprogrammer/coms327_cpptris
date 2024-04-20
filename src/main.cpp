#include <gamemanager.h>
#include <leaderboard.h>

int main() {
    GameManager manager = GameManager();
    int level;
    
    while((level = manager.SelectLevel()) != -1) manager.StartGame(level);

    return 0;
}