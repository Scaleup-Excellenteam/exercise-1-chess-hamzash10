//
// Created by 2022 on 6/8/2024.
//

#include "Queen.h"

//bishop and rook will be ignored
Queen::Queen(const Player &color, const Location &location) : Piece('q',color, location),
                                                            Bishop(NoColor,location),Rook(NoColor,location){

}

void Queen::move(const Location &destination) {
    if(Rook::is_legal_move(destination))
        Rook::move(destination);
    else
        Bishop::move(destination);
}

bool Queen::is_legal_move(const Location &destination) {
    return Rook::is_legal_move(destination) || Bishop::is_legal_move(destination);
}
