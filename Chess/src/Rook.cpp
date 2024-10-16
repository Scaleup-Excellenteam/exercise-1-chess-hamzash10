//
// Created by 2022 on 6/8/2024.
//

#include <iostream>
#include "../include/Rook.h"

Rook::Rook(const Player& color,const Location& location): Piece('r',color,location) {

}

void Rook::move(const Location& destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Rook::is_legal_move(const Location& destination) {
    if((_location.x==destination.x && _location.y!=destination.y) ||
            (_location.x!=destination.x && _location.y==destination.y))
        return true;
    return false;
}

