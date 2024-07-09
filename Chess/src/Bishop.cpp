//
// Created by 2022 on 6/8/2024.
//


#include <cmath>
#include "../include/Bishop.h"

Bishop::Bishop(const Player &color, const Location &location):Piece('b',color,location) {

}

void Bishop::move(const Location &destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Bishop::is_legal_move(const Location &destination) {
    // since the bishop moves diagonally, I can calculate the slope of the line connecting the current location and the destination,
    // meaning deltaX must be equal to deltaY
    // am I proud of my self?
    // yes, yes I'm
    int deltaX = std::abs(destination.x - _location.x);
    int deltaY = std::abs(destination.y - _location.y);

    return deltaX == deltaY;
}
