//
// Created by 2022 on 6/9/2024.
//

#include "Knight.h"

Knight::Knight(const Player &color, const Location &location):Piece('k',color,location) {

}

void Knight::move(const Location &destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Knight::is_legal_move(const Location &destination) {

    if((_location.x-1==destination.x && _location.y-2==destination.y) || (_location.x+1==destination.x && _location.y-2==destination.y) ||
            (_location.x+2==destination.x && _location.y-1==destination.y) ||(_location.x+2==destination.x && _location.y+1==destination.y) ||
            (_location.x+1==destination.x && _location.y+2==destination.y) ||(_location.x-1==destination.x && _location.y+2==destination.y) ||
            (_location.x-2==destination.x && _location.y+1==destination.y) ||(_location.x-2==destination.x && _location.y-1==destination.y))
            return true;
    return false;
}
