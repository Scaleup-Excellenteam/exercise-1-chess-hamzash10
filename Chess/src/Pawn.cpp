//
// Created by 2022 on 6/9/2024.
//

#include "../include/Pawn.h"

Pawn::Pawn(const Player &color, const Location &location): Piece('p',color,location) {

}

void Pawn::move(const Location &destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Pawn::is_legal_move(const Location &destination) {
    int legal_moves=1;
    if(color==White && _location.y==1 || color==Black && _location.y==6)
        legal_moves++;

    if(color==White) {
        if (_location.x == destination.x && destination.y>_location.y && _location.y + legal_moves >= destination.y)
            return true;
    }else{
        if (_location.x == destination.x && destination.y<_location.y && _location.y - legal_moves <= destination.y)
            return true;
    }


    return false;
}

bool Pawn::is_legal_move(shared_ptr<Piece> piece_to_attack) {
    if(piece_to_attack->get_type()=='#')
        return is_legal_move(piece_to_attack->get_location());

    Location destination=piece_to_attack->get_location();
    if(color==White) {
        if(piece_to_attack->get_color()==Black &&
        (_location.y+1==destination.y && (_location.x-1==destination.x || _location.x+1==destination.x)))
            return true;
    }else if(color==Black) {
        if(piece_to_attack->get_color()==White &&
           (_location.y-1==destination.y && (_location.x-1==destination.x || _location.x+1==destination.x)))
            return true;
    }
    return false;
}

vector<shared_ptr<Location>> Pawn::all_possible_moves() {
    vector<shared_ptr<Location>> possible_moves;

    int legal_moves = (color == Player::White) ? 1 : -1;

    // Forward movement
    Location forward_one = {_location.x, _location.y + legal_moves};
    if (is_legal_move(forward_one)) {
        possible_moves.push_back(make_shared<Location>(forward_one));
    }

    // Double forward movement on first move
    Location forward_two = {_location.x, _location.y + 2 * legal_moves};
    if (is_legal_move(forward_two)) {
        possible_moves.push_back(make_shared<Location>(forward_two));
    }

    // Diagonal captures
    Location diagonal_left = {_location.x - 1, _location.y + legal_moves};
    Location diagonal_right = {_location.x + 1, _location.y + legal_moves};
    if (is_legal_move(diagonal_left)) {
        possible_moves.push_back(make_shared<Location>(diagonal_left));
    }
    if (is_legal_move(diagonal_right)) {
        possible_moves.push_back(make_shared<Location>(diagonal_right));
    }
    return possible_moves;
}
