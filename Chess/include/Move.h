//
// Created by 2022 on 7/13/2024.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H
#include <iostream>
#include "Board.h"
using namespace std;

class Move {
    Board& board;
    static shared_ptr<Move> instance;
    Move(Board& b);
public:
    Move(const Move&) = delete;
    Move& operator=(const Move&) = delete;
    static Move& getInstance(Board& b);
    static Move& getInstance();
    friend ostream& operator<<(std::ostream& os, const Move& move);
};


#endif //CHESS_MOVE_H
