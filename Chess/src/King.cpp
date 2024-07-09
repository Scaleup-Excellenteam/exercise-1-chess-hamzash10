//
// Created by 2022 on 6/8/2024.
//

#include "../include/King.h"

King::King(const Player& color,const Location& location): Piece('k',color,location) {

}

void King::move(const Location& destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool King::is_legal_move(const Location& destination) {
    if((_location.x==destination.x && _location.y==destination.y-1) || (_location.x==destination.x-1 && _location.y==destination.y-1)
        || (_location.x==destination.x+1 && _location.y==destination.y-1) || (_location.x==destination.x-1 && _location.y==destination.y)
        || (_location.x==destination.x+1 && _location.y==destination.y) || (_location.x==destination.x && _location.y==destination.y+1)
        || (_location.x==destination.x-1 && _location.y==destination.y+1) || (_location.x==destination.x+1 && _location.y==destination.y+1))
        return true;
    return false;
}


