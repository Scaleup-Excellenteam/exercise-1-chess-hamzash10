//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H


#include "Rook.h"
#include "Bishop.h"

// I created the queen as a combination of a rook and a bishop
// this creates diamond inheritance
// I'm i proud of my self?
// Very much so.
class Queen: public Rook,public Bishop {
public:
    Queen(const Player& color,const Location& location);
    void move(const Location& destination) override;
    bool is_legal_move(const Location& destination) override;
    vector<shared_ptr<Location>> all_possible_moves() override;
};


#endif //CHESS_QUEEN_H
