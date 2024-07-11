//
// Created by 2022 on 6/8/2024.
//

#include "../include/King.h"

King::King(const Player& color,const Location& location): Piece('k',color,location),did_move(false) {

}

void King::move(const Location& destination) {
    _location.y=destination.y;
    _location.x=destination.x;
    did_move=true;
}

bool King::is_legal_move(const Location& destination) {
    //check for castling
    if(is_first_move() && abs(_location.x-destination.x)==2 && _location.y-destination.y==0)
        return true;
    //if the move is one square further in any direction
    return abs(_location.x-destination.x)<=1 && abs(_location.y-destination.y)<=1;
}

bool King::is_first_move() {
    return !did_move;
}

vector<shared_ptr<Location>> King::all_possible_moves() {
    vector<shared_ptr<Location>> possible_moves;

    // all king possible moves
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            if (x == 0 && y == 0) continue;
            Location new_location = {_location.x + x, _location.y + y};
            if (new_location.x >= BOARD_MIN_PLACE && new_location.x < BOARD_MAX_PLACE &&
                new_location.y >= BOARD_MIN_PLACE && new_location.y < BOARD_MAX_PLACE) {
                possible_moves.push_back(make_shared<Location>(new_location));
            }
        }
    }

    // Add castling moves
    if (is_first_move()) {
        possible_moves.push_back(make_shared<Location>(_location.x - 2, _location.y));
        possible_moves.push_back(make_shared<Location>(_location.x + 2, _location.y));
    }

    return possible_moves;
}


