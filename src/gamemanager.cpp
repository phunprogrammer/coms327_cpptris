#include <gamemanager.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

GameManager::GameManager() : lines(0), score(0) {
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

    refresh();
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
    InitGame(level);
    IncrementCount((blockEnum)game.SpawnBlock());
    UpdateScreen();

    auto newTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;
    int input = 0;

    while(true) {
        newTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime).count();
        currentTime = newTime;
        accumulator += deltaTime;

        if (deltaTime < MILLIS_PER_FRAME) std::this_thread::sleep_for(std::chrono::milliseconds((int)(MILLIS_PER_FRAME - deltaTime)));

        input = HandleInput(getch(), accumulator);

        if(input == 2 || input == -1) break;
        else if(input) flushinp();

        if (accumulator >= GetSpeed()) {
            if(!IncrementLines(game.Drop())) {
                std::this_thread::sleep_for(std::chrono::milliseconds(DROP_PAUSE));
                accumulator -= DROP_PAUSE;
                if(!IncrementCount((blockEnum)game.SpawnBlock())) {
                    UpdateScreen();
                    break;
                }
            }

            UpdateScreen();

            accumulator -= GetSpeed();
        }

        PrintTime();
    }

    if (input != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        PrintEnd();
    }

    return input;
}

int GameManager::PrintBoard() {
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
    delwin(gameWin);
    return 1;
}

int GameManager::HandleInput(int input, double& accumulator) {
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
            if(!IncrementLines(game.Drop())) {
                std::this_thread::sleep_for(std::chrono::milliseconds(DROP_PAUSE));
                accumulator -= DROP_PAUSE;
                if(!IncrementCount((blockEnum)game.SpawnBlock())) {
                    UpdateScreen();
                    return 2;
                }
            }
            else score++;

            break;
        case KEY_BACKSPACE:
            return -1;
        case 10:
            accumulator -= PrintPause();
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
        speed = 48 - level * 5;

    return 1000 * (speed / 60);
}

int GameManager::IncrementLines(int lines) {
    if(!lines) return 0;
    this->lines += lines - 1;

    if(this->lines >= (level * 10 + 10)) {
        level++;
    }

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

    return lines == 1 ? 1 : 0;
}

int GameManager::UpdateScreen() {
    PrintBoard();
    PrintLines();
    PrintScores();
    PrintNext();
    PrintLevel();
    PrintCount();

    return 1;
}

int GameManager::PrintLines() {
    lineWin = newwin(LINES_HEIGHT, BOARD_COLS + 1, BORDER, 51);
    box(lineWin, 0, 0);

    mvwprintw(lineWin, 1, 2, "LINES:");
    mvwprintw(lineWin, 2, 2, "%02d", lines);

    wrefresh(lineWin);
    delwin(lineWin);
    return 1;
}

int GameManager::PrintScores() {
    scoreWin = newwin(SCORE_HEIGHT, BOARD_COLS + 1, BORDER + LINES_HEIGHT, 51);
    box(scoreWin, 0, 0);

    mvwprintw(scoreWin, 1, 2, "SCORE:");
    mvwprintw(scoreWin, 2, 2, "%07d", score);

    wrefresh(scoreWin);
    delwin(scoreWin);
    return 1;
}

int GameManager::PrintNext() {
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
    delwin(nextWin);
    return 1;
}

int GameManager::PrintLevel() {
    levelWin = newwin(LEVELS_HEIGHT, BOARD_COLS + 1, BORDER + LINES_HEIGHT + SCORE_HEIGHT + NEXT_HEIGHT, 51);
    box(levelWin, 0, 0);

    mvwprintw(levelWin, 1, 2, "LEVEL:");
    mvwprintw(levelWin, 2, 2, "%02d", level);

    wrefresh(levelWin);
    delwin(levelWin);
    return 1;
}

int GameManager::SelectLevel() {
    menuWin = newwin(24, 80, 0,0);

    std::vector<std::vector<int>> levels;

    for(int y = 0; y < 3; y++) {
        levels.push_back(std::vector<int>());
        for(int x = 0; x < 10; x++) {
            levels[y].push_back(y * 10 + x);
        }
    }

    coord_t selection = { 0, 0 };

    int input = 0;

    box(menuWin, 0, 0);
    PrintLogo();
    PrintLeaderBoard();
    mvwprintw(menuWin, 8, 33, "Select a level:");
    wrefresh(menuWin);

    do {
        if(input == ERR)
            continue;

        switch(input) {
            case KEY_RIGHT:
                selection.x + 1 < (int)levels[selection.y].size() ? selection.x++ : selection.x;
                break;
            case KEY_LEFT:
                selection.x - 1 >= 0 ? selection.x-- : selection.x;
                break;
            case KEY_DOWN:
                selection.y + 1 < (int)levels.size() && selection.x < (int)levels[selection.y + 1].size() ? selection.y++ : selection.y;
                break;
            case KEY_UP:
                selection.y - 1 >= 0 ? selection.y-- : selection.y;
                break;
            case KEY_BACKSPACE:
                return -1;
        }

        for(int i = 0; i < (int)levels.size(); i++) {
            for(int j = 0; j < (int)levels[i].size(); j++) {
                if(selection.y == i && selection.x == j) {
                    mvwprintw(menuWin, 8 + (i + 1), j * 6 + 11, ">%02d<", levels[i][j]);
                }
                else 
                    mvwprintw(menuWin, 8 + (i + 1), j * 6 + 11, " %02d ", levels[i][j]);
            }
        }

        wrefresh(menuWin);
        std::this_thread::sleep_for(std::chrono::milliseconds((int)MILLIS_PER_FRAME));
    } while((input = getch()) != 10);

    wclear(menuWin);
    box(menuWin, 0, 0);
    wrefresh(menuWin);
    delwin(menuWin);

    return levels[selection.y][selection.x];
}

int GameManager::IncrementCount(blockEnum block) {
    if(block == NULL_BLOCK) return 0;
    count[block]++;
    return block;
}

int GameManager::PrintCount() {
    countWin = newwin(22, BOARD_COLS + 5, BORDER, 29 - (BOARD_COLS + 5));
    box(countWin, 0, 0);

    for(int i = 0; i < (int)BLOCKS.size(); i++) {
        const auto blockBits = game.getBitSet({BLOCKS[i], 0, {0, 0}});

        for(int y = 1; y < BLOCK_WIDTH - 1; y++)
            for(int x = 0; x < BLOCK_LENGTH; x++) {
                int color = 0;
                if(!blockBits[y * BLOCK_LENGTH + x])
                    color = BLOCKS.size() + 1;
                else
                    color = BLOCKS[i];

                wattron(countWin, COLOR_PAIR(color));
                mvwprintw(countWin, 1 + (y - 1) + i * 3, 1 + x * 2, "██");
                wattroff(countWin, COLOR_PAIR(color));
            }

        mvwprintw(countWin, i * 3 + 1, 11, "%03d", count.at(BLOCKS[i]));
    }

    wrefresh(countWin);
    delwin(countWin);
    return 1;
}

int GameManager::InitTimeWin() {
    timeWin = newwin(3, BOARD_COLS + 1, BORDER + LINES_HEIGHT + SCORE_HEIGHT + NEXT_HEIGHT + LEVELS_HEIGHT, 51);
    box(timeWin, 0, 0);

    wrefresh(timeWin);
    return 1;
}

int GameManager::PrintTime() {
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - time;
    mvwprintw(timeWin, 1, 2, "%7.0lf", duration.count());
    wrefresh(timeWin);

    return 1;
}

int GameManager::InitGame(int level) {
    game = Game();

    InitTimeWin();
    time = std::chrono::high_resolution_clock::now();
    
    for(auto& [key, value] : count)
        value = 0;

    this->lines = 0;
    this->score = 0;
    this->level = level;

    return 1;
}

int GameManager::PrintLogo() {
    int marginY = 1;
    int marginX = 17;
    std::ifstream logo("./data/logo.txt");

    if(!logo.is_open()) return 0;

    std::string line;
    int lineIndex = 0;
    while (std::getline(logo, line))
        mvwaddstr(menuWin, marginY + lineIndex++, marginX, line.c_str());

    logo.close();
    wrefresh(menuWin);
    return 1;
}

int GameManager::PrintLeaderBoard() {
    boardWin = newwin(11, 24, 12, 28);
    box(boardWin, 0, 0);
    mvwprintw(boardWin, 1, 1, "    NAME     SCORE");

    int i = 1;
    for(auto& entry : board.getEntries()) {
        mvwprintw(boardWin, 1 + i, 1, " %d. %3s      %07d", i, entry.getData().c_str(), entry.getPriority());
        i++;
    }

    wrefresh(boardWin);
    delwin(boardWin);
    return 1;
}

int GameManager::PrintEnd() {
    int width = 22;
    int height = 5;

    nodelay(stdscr, FALSE);
    endWin = newwin(height, width, 12 - (height / 2), 40 - (width / 2));
    box(endWin, 0, 0);

    if(board.getEntries().size() > 0 && board.getEntries().size() >= LEADERBOARD_SIZE && score <= (*--board.getEntries().end()).getPriority()) {
        mvwprintw(endWin, 2, 7, "NICE TRY!");
        wrefresh(endWin);
        while(getch() != 10);
        nodelay(stdscr, TRUE);
        return 0;
    }

    std::string name;
    int input;
    mvwprintw(endWin, 2, 1, "YOU'RE ON THE BOARD!");
    wrefresh(endWin);
    while(getch() != 10);

    curs_set(1);

    mvwprintw(endWin, 2, 1, "                    ");
    mvwprintw(endWin, 2, 6, "NAME: ");
    wrefresh(endWin);

    int start = 12;
    int cursor = start;

    while((input = getch()) != 10 || (int)name.size() == 0) {
        switch(input){
            case KEY_BACKSPACE:
                if (!name.empty()) {
                    name.erase(name.end() - 1);
                    cursor--;
                }
                break;
            default:
                if(std::isalpha(input) && (int)name.size() < 3) {
                    name += std::toupper(input);
                    cursor++;
                }
                break;
        }

        mvwprintw(endWin, 2, start, "   ");
        mvwprintw(endWin, 2, start, "%s", name.c_str());
        wmove(endWin, 2, cursor);
        wrefresh(endWin);
    }
    board.Insert(name, score);
    board.Save();

    delwin(endWin);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
    return 1;
}

int GameManager::PrintPause() {
    auto current = std::chrono::high_resolution_clock::now();
    int width = 22;
    int height = 5;

    nodelay(stdscr, FALSE);
    pauseWin = newwin(height, width, 12 - (height / 2), 40 - (width / 2));
    box(pauseWin, 0, 0);

    mvwprintw(pauseWin, 2, 9, "PAUSE");
    wrefresh(pauseWin);
    while(getch() != 10);
    nodelay(stdscr, TRUE);

    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - current).count();
}