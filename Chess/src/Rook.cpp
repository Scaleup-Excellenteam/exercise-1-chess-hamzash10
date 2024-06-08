//
// Created by 2022 on 6/8/2024.
//

#include "Rook.h"

Rook::Rook(const string& color,const Location& location): Piece('r',color,location) {

}

int Rook::move(const Location& destination) {
    //21 - illegal movement of that piece
    if(!is_legal_move(destination))
        return 21;

    //41 - the last movement was legal and cause check
    //42 - the last movement was legal, next turn
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Rook::is_legal_move(const Location& destination) {
    if((_location.x==destination.x && _location.y!=destination.y) ||
            (_location.x!=destination.x && _location.y==destination.y))
        return true;
    return false;
}

