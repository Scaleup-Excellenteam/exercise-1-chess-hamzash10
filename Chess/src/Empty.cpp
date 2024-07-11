//
// Created by 2022 on 6/8/2024.
//

#include "../include/Empty.h"

Empty::Empty(const Player &color, const Location& location): Piece('#',color,location) {

}

void Empty::move(const Location& destination) {

}

bool Empty::is_legal_move(const Location& destination) {
    return true;
}

vector<shared_ptr<Location>> Empty::all_possible_moves() {
    return vector<shared_ptr<Location>>();
}

