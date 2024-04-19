#include <vector>
#include <bitset>
#include <map>

#define BOARD_ROWS 22
#define BOARD_COLS 10

#ifndef GAME_H
#define GAME_H

enum blockEnum : int {
    I = 1,
    O = 2,
    L = 3,
    J = 4,
    S = 5,
    Z = 6,
    T = 7
};

const std::vector<blockEnum> BLOCKS({ I, O, L, J, S, Z, T });

const std::vector<std::bitset<16>> I_ROTATIONS ({
    std::bitset<16>{0b0000000011110000}, 
    std::bitset<16>{0b0100010001000100}
});

const std::vector<std::bitset<16>> O_ROTATIONS ({
    std::bitset<16>{0b0000011001100000}
});

const std::vector<std::bitset<16>> L_ROTATIONS ({
    std::bitset<16>{0b0000001011100000},
    std::bitset<16>{0b0000010001000110},
    std::bitset<16>{0b0000000011101000},
    std::bitset<16>{0b0000110001000100}
});

const std::vector<std::bitset<16>> J_ROTATIONS ({
    std::bitset<16>{0b0000100011100000},
    std::bitset<16>{0b0000011001000100},
    std::bitset<16>{0b0000000011100010},
    std::bitset<16>{0b0000010001001100}
});

const std::vector<std::bitset<16>> S_ROTATIONS ({
    std::bitset<16>{0b0000011011000000},
    std::bitset<16>{0b0000100011000100}
});

const std::vector<std::bitset<16>> Z_ROTATIONS ({
    std::bitset<16>{0b0000110001100000},
    std::bitset<16>{0b0000010011001000}
});

const std::vector<std::bitset<16>> T_ROTATIONS ({
    std::bitset<16>{0b0000010011100000},
    std::bitset<16>{0b0000010001100100},
    std::bitset<16>{0b0000000011100100},
    std::bitset<16>{0b0000010011000100}
});

const std::map<blockEnum, std::vector<std::bitset<16>>> ROTATIONS ({
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
        std::pair<blockEnum, int> currentBlock;
        
        int score;
        float time;
    public:
        Game();
};

#endif