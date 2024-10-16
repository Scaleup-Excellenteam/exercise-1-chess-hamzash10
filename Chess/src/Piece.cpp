//
// Created by 2022 on 6/8/2024.
//

#include "../include/Piece.h"

Piece::Piece(const char &piece_type,const Player& color, const Location& location):color(color),_location(location) {
    this->piece_type=(color==White)? toupper(piece_type): tolower(piece_type);
}

Location Piece::get_location() {
    return _location;
}

char Piece::get_type() const {
    return piece_type;
}

Player Piece::get_color() const {
    return color;
}
