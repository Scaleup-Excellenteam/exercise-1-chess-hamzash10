//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H


#include "Piece.h"

class Rook: virtual public Piece {
    bool did_move;
public:
    Rook(const Player& color,const Location& location);
    void move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;
    bool is_first_move();
    vector<shared_ptr<Location>> all_possible_moves() override;
};


#endif //CHESS_ROOK_H
