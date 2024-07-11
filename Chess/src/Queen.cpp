//
// Created by 2022 on 6/8/2024.
//

#include "../include/Queen.h"

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

vector<shared_ptr<Location>> Queen::all_possible_moves() {
    vector<shared_ptr<Location>> possible_moves = Rook::all_possible_moves();
    vector<shared_ptr<Location>> bishop_moves = Bishop::all_possible_moves();

    // Add all bishop possible moves to the possible_moves vector that already have the rook's moves
    possible_moves.insert(possible_moves.end(), bishop_moves.begin(), bishop_moves.end());

    return possible_moves;
}
