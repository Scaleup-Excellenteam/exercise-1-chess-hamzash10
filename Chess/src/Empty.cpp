//
// Created by 2022 on 6/8/2024.
//

#include "Empty.h"

Empty::Empty(const string &color, const Location& location): Piece('#',color,location) {

}

int Empty::move(const Location& destination) {

}

bool Empty::is_legal_move(const Location& destination) {
    return false;
}

