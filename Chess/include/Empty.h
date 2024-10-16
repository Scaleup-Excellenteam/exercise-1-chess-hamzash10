//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_EMPTY_H
#define CHESS_EMPTY_H


#include "Piece.h"

class Empty: public Piece{
public:
    Empty(const Player& color,const Location& location);
    void move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;
    vector<shared_ptr<Location>> all_possible_moves() override;
};


#endif //CHESS_EMPTY_H
