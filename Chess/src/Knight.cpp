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

vector<shared_ptr<Location>> Knight::all_possible_moves() {
    vector<shared_ptr<Location>> possible_moves;

    // all knight leagal moves
    vector<pair<int,int>> legal_moves={
            {-2, -1},
            {-2, +1},
            {-1, +2},
            {+1, +2},
            {+2, +1},
            {+2, -1},
            {+1, -2},
            {-1, -2}
    };

    for(auto move:legal_moves){
        Location new_location={_location.x + move.first, _location.y + move.second};
        if (new_location.x >= BOARD_MIN_PLACE && new_location.x < BOARD_MAX_PLACE &&
            new_location.y >= BOARD_MIN_PLACE && new_location.y < BOARD_MAX_PLACE)
            possible_moves.push_back(make_shared<Location>(new_location));
    }
    return possible_moves;
}
