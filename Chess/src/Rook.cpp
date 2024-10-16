//
// Created by 2022 on 6/8/2024.
//

#include <iostream>
#include "../include/Rook.h"

Rook::Rook(const Player& color,const Location& location): Piece('r',color,location),did_move(false) {

}

void Rook::move(const Location& destination) {
    _location.y=destination.y;
    _location.x=destination.x;
    did_move=true;
}

bool Rook::is_legal_move(const Location& destination) {
    if((_location.x==destination.x && _location.y!=destination.y) ||
            (_location.x!=destination.x && _location.y==destination.y))
        return true;
    return false;
}

bool Rook::is_first_move() {
    return !did_move;
}

vector<shared_ptr<Location>> Rook::all_possible_moves() {
    vector<shared_ptr<Location>> possible_moves;

    // all rook horizontal moves
    for (int x = BOARD_MIN_PLACE; x < BOARD_MAX_PLACE; ++x) {
        if(x==_location.x) continue;
        Location new_location = {x, _location.y};
        if (new_location.x >= BOARD_MIN_PLACE && new_location.x < BOARD_MAX_PLACE) {
            possible_moves.push_back(make_shared<Location>(new_location));
        }
    }
    //all rook vertical moves
    for (int y = BOARD_MIN_PLACE; y < BOARD_MAX_PLACE; ++y) {
        if (y == _location.y) continue;
        Location new_location = {_location.x, y};
        if (new_location.y >= BOARD_MIN_PLACE && new_location.y < BOARD_MAX_PLACE) {
            possible_moves.push_back(make_shared<Location>(new_location));
        }
    }
    return possible_moves;
}
