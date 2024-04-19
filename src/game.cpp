#include <game.h>

Game::Game() : board(BOARD_ROWS, std::vector<int>(BOARD_COLS, 0)) {
    srand(time(NULL));
    SpawnBlock();
}

int Game::SpawnBlock() {
    return SpawnBlock((blockEnum)(rand() % BLOCKS.size() + 1));
}

int Game::SpawnBlock(blockEnum block) {
    current = { block, 0, { BOARD_SPAWN_X, BOARD_SPAWN_Y } };
    
    if(!CheckBlock(current))
        return 0;

    PlaceBlock(current);

    return 1;
}

int Game::PlaceBlock(block_t block) {
    coord_t start = { block.coord.x - BLOCK_CENTER_X, block.coord.y - BLOCK_CENTER_Y };
    const auto blockBits = getBitSet(block);

    for(int y = 0; y < BLOCK_WIDTH; y++)
        for(int x = 0; x < BLOCK_LENGTH; x++)
            if(blockBits[y * BLOCK_LENGTH + x] && !board[start.y + y][start.x + x])
                board[start.y + y][start.x + x] = block.type;

    return 1;
}

int Game::CheckBlock(block_t block) {
    coord_t start = { block.coord.x - BLOCK_CENTER_X, block.coord.y - BLOCK_CENTER_Y };
    const auto blockBits = getBitSet(block);

    for(int y = 0; y < BLOCK_WIDTH; y++) {
        for(int x = 0; x < BLOCK_LENGTH; x++) {
            if(start.x + x < 0 || start.x + x >= BOARD_COLS || start.y + y < 0 || start.y + y >= BOARD_ROWS) {
                if(blockBits[y * BLOCK_LENGTH + x]) {
                    return 0;
                }
                else continue;
            }
        
            if(blockBits[y * BLOCK_LENGTH + x] && board[start.y + y][start.x + x])
                return 0;
        }
    }

    return 1;
}

const std::bitset<BITS> Game::getBitSet(block_t block) {
    std::vector<std::bitset<BITS>> vector = ROTATIONS.at(block.type);
    return vector[block.rotation % vector.size()];
}

int Game::DeleteBlock(block_t block) {
    coord_t start = { block.coord.x - BLOCK_CENTER_X, block.coord.y - BLOCK_CENTER_Y };
    const auto blockBits = getBitSet(block);
    bool deleted = false;

    for(int y = 0; y < BLOCK_WIDTH; y++)
        for(int x = 0; x < BLOCK_LENGTH; x++) 
            if(blockBits[y * BLOCK_LENGTH + x]) {
                board[start.y + y][start.x + x] = 0;
                deleted = true;
            }

    if(!deleted)
        return 0;

    return 1;
}

int Game::Drop() {
    block_t drop = current;
    drop.coord.y++;

    return MoveBlock(drop);
}

int Game::MoveLeft() {
    block_t left = current;
    left.coord.x--;

    return MoveBlock(left);
}

int Game::MoveRight() {
    block_t right = current;
    right.coord.x++;

    return MoveBlock(right);
}

int Game::RotateCW() {
    block_t cw = current;
    cw.rotation++;

    return MoveBlock(cw);
}

int Game::RotateCCW() {
    block_t ccw = current;
    ccw.rotation--;

    return MoveBlock(ccw);
}

int Game::MoveBlock(block_t block) {
    DeleteBlock(current);

    if(!CheckBlock(block)) {
        PlaceBlock(current);
        return 0;
    }

    PlaceBlock(block);
    current = block;
    return 1;
}