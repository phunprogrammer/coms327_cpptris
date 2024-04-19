#include <gamemanager.h>
#include <iostream>
#include <chrono>
#include <thread>

#define COLOR_ORANGE 8
#define FRAMES 60

#define BORDER 1
#define LINES_HEIGHT 4
#define SCORE_HEIGHT 4
#define NEXT_HEIGHT 7
#define LEVELS_HEIGHT 4

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

    for(int i = 0; i < (int)BLOCKS.size(); i++)
        count[BLOCKS[i]] = 0;
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

    auto startTime = std::chrono::steady_clock::now();

    while(1) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        int input = HandleInput(getch());

        if(input == 2) break;
        else if(input) flushinp();

        if (elapsedTime >= GetSpeed()) {
            startTime = currentTime;

            if(!IncrementLines(game.Drop()) && !IncrementCount((blockEnum)game.SpawnBlock()))
                break;

            UpdateScreen();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FRAMES));
    }

    return 1;
}

int GameManager::PrintBoard() {
    delwin(gameWin);
    gameWin = newwin(BOARD_ROWS - BOARD_SPAWN_Y + 2, BOARD_COLS * 2 + 2, BORDER, 29);
    box(gameWin, 0, 0);

    for(int y = BOARD_SPAWN_Y; y < BOARD_ROWS; y++)
        for(int x = 0; x < BOARD_COLS; x++) {
            int color;

            if(!game.getBoard()[y][x])
                color = BLOCKS.size() + 1;
            else
                color = game.getBoard()[y][x];

            wattron(gameWin, COLOR_PAIR(color));
            mvwprintw(gameWin, y - 1, x * 2 + 1, "██");
            wattroff(gameWin, COLOR_PAIR(color));
        }

    wrefresh(gameWin);

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
            if(!IncrementLines(game.Drop()) && !IncrementCount((blockEnum)game.SpawnBlock()))
                return 2;
            break;
        default:
            return 0;
    }

    if(out)
        UpdateScreen();

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

int GameManager::UpdateScreen() {
    box(menuWin, 0, 0);
    wrefresh(menuWin);
    PrintBoard();
    PrintLines();
    PrintScores();
    PrintNext();
    PrintLevel();

    return 1;
}

int GameManager::PrintLines() {
    delwin(lineWin);
    lineWin = newwin(LINES_HEIGHT, BOARD_COLS + 1, BORDER, 51);
    box(lineWin, 0, 0);

    mvwprintw(lineWin, 1, 2, "LINES:");
    mvwprintw(lineWin, 2, 2, "%02d", lines);

    wrefresh(lineWin);
    return 1;
}

int GameManager::PrintScores() {
    delwin(scoreWin);
    scoreWin = newwin(SCORE_HEIGHT, BOARD_COLS + 1, BORDER + LINES_HEIGHT, 51);
    box(scoreWin, 0, 0);

    mvwprintw(scoreWin, 1, 2, "SCORE:");
    mvwprintw(scoreWin, 2, 2, "%07d", score);

    wrefresh(scoreWin);
    return 1;
}

int GameManager::PrintNext() {
    delwin(nextWin);
    nextWin = newwin(NEXT_HEIGHT, BOARD_COLS + 1, BORDER + LINES_HEIGHT + SCORE_HEIGHT, 51);
    box(nextWin, 0, 0);

    const auto blockBits = game.getBitSet({game.getNext(), 0, {0, 0}});

    mvwprintw(nextWin, 1, 2, "NEXT:");

    for(int y = 0; y < BLOCK_WIDTH; y++)
        for(int x = 0; x < BLOCK_LENGTH; x++) {
            int color = 0;
            if(!blockBits[y * BLOCK_LENGTH + x])
                color = BLOCKS.size() + 1;
            else
                color = game.getNext();

            wattron(nextWin, COLOR_PAIR(color));
            mvwprintw(nextWin, y + 2, x * 2 + 1, "██");
            wattroff(nextWin, COLOR_PAIR(color));
        }

    wrefresh(nextWin);
    return 1;
}

int GameManager::PrintLevel() {
    delwin(levelWin);
    levelWin = newwin(LEVELS_HEIGHT, BOARD_COLS + 1, BORDER + LINES_HEIGHT + SCORE_HEIGHT + NEXT_HEIGHT, 51);
    box(levelWin, 0, 0);

    mvwprintw(levelWin, 1, 2, "LEVEL:");
    mvwprintw(levelWin, 2, 2, "%02d", level);

    wrefresh(levelWin);
    return 1;
}

int GameManager::SelectLevel() {
    delwin(menuWin);
    menuWin = newwin(24, 80, 0,0);
    box(menuWin, 0, 0);

    wrefresh(menuWin);
    return 1;
}

int GameManager::IncrementCount(blockEnum block) {
    if(block == NULL_BLOCK) return 0;
    count[block]++;
    return block;
}
