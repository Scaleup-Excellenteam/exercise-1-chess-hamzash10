//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H


#include "Piece.h"

class Bishop : public Piece{
public:
    Bishop(const Player& color,const Location& location);
    void move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;

};


#endif //CHESS_BISHOP_H
