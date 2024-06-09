//
// Created by 2022 on 6/9/2024.
//

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H


#include "Piece.h"

class Pawn : public Piece{
public:
    Pawn(const Player& color,const Location& location);
    void move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;
    bool is_legal_move(shared_ptr<Piece> piece_to_attack);
};


#endif //CHESS_PAWN_H
