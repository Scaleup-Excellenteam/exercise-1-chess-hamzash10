//
// Created by 2022 on 6/9/2024.
//

#include "../include/Knight.h"

Knight::Knight(const Player &color, const Location &location):Piece('n',color,location) {

}

void Knight::move(const Location &destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Knight::is_legal_move(const Location &destination) {
    int deltaX = std::abs(_location.x - destination.x);
    int deltaY = std::abs(_location.y - destination.y);

    // Knight moves in an "L" shape: 2 squares in one direction and 1 square perpendicular
    return (deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1);
}
