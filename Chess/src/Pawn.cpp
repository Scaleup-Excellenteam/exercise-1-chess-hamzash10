//
// Created by 2022 on 6/9/2024.
//

#include "../include/Pawn.h"

Pawn::Pawn(const Player &color, const Location &location): Piece('p',color,location) {

}

void Pawn::move(const Location &destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Pawn::is_legal_move(const Location &destination) {
    int legal_moves=1;
    if(color==White && _location.y==1 || color==Black && _location.y==6)
        legal_moves++;

    if(color==White) {
        if (_location.x == destination.x && destination.y>_location.y && _location.y + legal_moves >= destination.y)
            return true;
    }else{
        if (_location.x == destination.x && destination.y<_location.y && _location.y - legal_moves <= destination.y)
            return true;
    }


    return false;
}

bool Pawn::is_legal_move(shared_ptr<Piece> piece_to_attack) {
    if(piece_to_attack->get_type()=='#')
        return is_legal_move(piece_to_attack->get_location());

    Location destination=piece_to_attack->get_location();
    if(color==White) {
        if(piece_to_attack->get_color()==Black &&
        (_location.y+1==destination.y && (_location.x-1==destination.x || _location.x+1==destination.x)))
            return true;
    }else if(color==Black) {
        if(piece_to_attack->get_color()==White &&
           (_location.y-1==destination.y && (_location.x-1==destination.x || _location.x+1==destination.x)))
            return true;
    }
    return false;
}
