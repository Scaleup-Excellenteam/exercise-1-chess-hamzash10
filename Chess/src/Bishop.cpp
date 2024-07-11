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

vector<shared_ptr<Location>> Bishop::all_possible_moves() {
    vector<shared_ptr<Location>> possible_moves;

    // all bishop directions
    vector<pair<int,int>> diagonals={
            {-1,-1}, // north-west
            {+1,-1}, // north-east
            {-1,+1}, // south-west
            {+1,+1} // south-east
    };

    for(auto direction:diagonals){
        int x=_location.x;
        int y=_location.y;

        while (true) {
            x+=direction.first;
            y+=direction.second;
            if (x < BOARD_MIN_PLACE || x >= BOARD_MAX_PLACE || y < BOARD_MIN_PLACE || y >= BOARD_MAX_PLACE) {
                break;
            }
            possible_moves.push_back(std::make_shared<Location>(x, y));
        }
    }
    return possible_moves;
}
