#include <gamemanager.h>

int main() {
    GameManager manager = GameManager();

    manager.StartGame(manager.SelectLevel());

    return 0;
}