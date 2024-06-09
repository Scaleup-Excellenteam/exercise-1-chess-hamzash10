//
// Created by 2022 on 6/9/2024.
//

#ifndef CHESS_KNIGHTS_H
#define CHESS_KNIGHTS_H


#include "Piece.h"

class Knight: public Piece {
public:
    Knight(const Player& color,const Location& location);
    void move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;
};


#endif //CHESS_KNIGHTS_H
