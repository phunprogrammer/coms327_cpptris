#include <vector>
#include <bitset>
#include <map>

#define BOARD_ROWS 22
#define BOARD_COLS 10
#define BOARD_SPAWN_X 5
#define BOARD_SPAWN_Y 2

#define BITS 16

#define BLOCK_LENGTH 4
#define BLOCK_WIDTH 4
#define BLOCK_CENTER_X 2
#define BLOCK_CENTER_Y 1

#ifndef GAME_H
#define GAME_H

typedef struct Coord {
    int x;
    int y;

    bool operator==(const Coord& compare) const {
        return this->x == compare.x && this->y == compare.y;
    }

    bool operator!=(const Coord& compare) const {
        return this->x != compare.x || this->y != compare.y;
    }

    bool operator<(const Coord& compare) const {
        if (this->x == compare.x) {
            return this->y < compare.y;
        }
        return this->x < compare.x;
    }
} coord_t;

enum blockEnum : int {
    I = 1,
    O = 2,
    L = 3,
    J = 4,
    S = 5,
    Z = 6,
    T = 7
};

typedef struct Block {
    blockEnum type;
    int rotation;
    coord_t coord;
} block_t;

const std::vector<blockEnum> BLOCKS({ I, O, L, J, S, Z, T });

const std::vector<std::bitset<BITS>> I_ROTATIONS ({
    std::bitset<BITS>{0b0000000011110000}, 
    std::bitset<BITS>{0b0100010001000100}
});

const std::vector<std::bitset<BITS>> O_ROTATIONS ({
    std::bitset<BITS>{0b0000011001100000}
});

const std::vector<std::bitset<BITS>> L_ROTATIONS ({
    std::bitset<BITS>{0b0000001011100000},
    std::bitset<BITS>{0b0000010001000110},
    std::bitset<BITS>{0b0000000011101000},
    std::bitset<BITS>{0b0000110001000100}
});

const std::vector<std::bitset<BITS>> J_ROTATIONS ({
    std::bitset<BITS>{0b0000100011100000},
    std::bitset<BITS>{0b0000011001000100},
    std::bitset<BITS>{0b0000000011100010},
    std::bitset<BITS>{0b0000010001001100}
});

const std::vector<std::bitset<BITS>> S_ROTATIONS ({
    std::bitset<BITS>{0b0000011011000000},
    std::bitset<BITS>{0b0000100011000100}
});

const std::vector<std::bitset<BITS>> Z_ROTATIONS ({
    std::bitset<BITS>{0b0000110001100000},
    std::bitset<BITS>{0b0000010011001000}
});

const std::vector<std::bitset<BITS>> T_ROTATIONS ({
    std::bitset<BITS>{0b0000010011100000},
    std::bitset<BITS>{0b0000010001100100},
    std::bitset<BITS>{0b0000000011100100},
    std::bitset<BITS>{0b0000010011000100}
});

const std::map<blockEnum, const std::vector<std::bitset<BITS>>> ROTATIONS ({
    { blockEnum::I, I_ROTATIONS },
    { blockEnum::O, O_ROTATIONS },
    { blockEnum::L, L_ROTATIONS },
    { blockEnum::J, J_ROTATIONS },
    { blockEnum::S, S_ROTATIONS },
    { blockEnum::Z, Z_ROTATIONS },
    { blockEnum::T, T_ROTATIONS }
});


class Game {
    private:
        std::vector<std::vector<int>> board;
        block_t current;
        int MoveBlock(block_t block);

    public:
        Game();
        int SpawnBlock();
        int SpawnBlock(blockEnum block);
        int DeleteBlock(block_t block);
        int CheckBlock(block_t block);
        int PlaceBlock(block_t block);

        int Drop();
        int MoveLeft();
        int MoveRight();
        int RotateCW();
        int RotateCCW();

        int ClearLine(int line);
        int ClearLines(block_t block);

        const auto& getBoard() const { return board; } 
        const std::bitset<BITS> getBitSet(block_t block);
};

#endif