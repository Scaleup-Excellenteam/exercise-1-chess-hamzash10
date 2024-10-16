//
// Created by 2022 on 7/13/2024.
//

#include "../include/Move.h"

shared_ptr<Move> Move::instance = nullptr;

Move::Move(Board &b):board(b) {

}

ostream& operator<<(std::ostream& os, const Move& move){
    os<<move.board;
    return os;
}

Move &Move::getInstance(Board &b) {
    if (!instance) {
        instance = shared_ptr<Move>(new Move(b));
    }
    return *instance;
}

Move &Move::getInstance() {
    return *instance;
}
