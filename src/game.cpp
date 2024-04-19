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
            if(blockBits[y * BLOCK_WIDTH + x] && !board[start.y + y][start.x + x])
                board[start.y + y][start.x + x] = block.type;

    return 1;
}

int Game::CheckBlock(block_t block) {
    coord_t start = { block.coord.x - BLOCK_CENTER_X, block.coord.y - BLOCK_CENTER_Y };
    const auto blockBits = getBitSet(current);

    for(int y = 0; y < BLOCK_WIDTH; y++) {
        for(int x = 0; x < BLOCK_LENGTH; x++) {
            if(start.x + x < 0 || start.x + x >= BOARD_COLS || start.y + y < 0 || start.y + y >= BOARD_ROWS) {
                if(blockBits[y * BLOCK_WIDTH + x]) return 0;
                else continue;
            }
        
            if(blockBits[y * BLOCK_WIDTH + x] && board[start.y + y][start.x + x])
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
            if(blockBits[y * BLOCK_WIDTH + x]) {
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

    DeleteBlock(current);

    if(!CheckBlock(drop)) {
        PlaceBlock(current);
        return 0;
    }

    PlaceBlock(drop);
    current = drop;
    return 1;
}