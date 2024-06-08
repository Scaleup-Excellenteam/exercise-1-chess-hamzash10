//
// Created by 2022 on 6/8/2024.
//

#include "King.h"

King::King(const string& color,const Location& location): Piece('k',color,location) {

}

int King::move(const Location& destination) {

}

bool King::is_legal_move(const Location& destination) {
    return false;
}


