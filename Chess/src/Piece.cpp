//
// Created by 2022 on 6/8/2024.
//

#include "Piece.h"

Piece::Piece(const char &piece_type,const string& color, const Location& location):piece_type(piece_type),color(color),_location(location) {

}

Location Piece::get_location() {
    return _location;
}

char Piece::get_type() const {
    return piece_type;
}

string Piece::get_color() const {
    return color;
}
