#include <gamemanager.h>
#include <ncurses.h>
#include <signal.h>

void do_resize(int num) {
    if(num) {}
    
    refresh();
}

int main() {
    signal(SIGWINCH, do_resize);

    GameManager manager = GameManager();
    int level;
    
    while((level = manager.SelectLevel()) != -1) {
        if(manager.StartGame(level) == -1)
            break;
    }

    return 0;
}