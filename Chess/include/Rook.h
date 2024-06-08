//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H


#include "Piece.h"

class Rook: public Piece {
public:
    Rook(const string& color,const Location& location);
    int move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;
};


#endif //CHESS_ROOK_H
