//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_KING_H
#define CHESS_KING_H


#include "Piece.h"

class King: public Piece {
public:
    King(const string& color,const Location& location);
    int move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;
};


#endif //CHESS_KING_H
