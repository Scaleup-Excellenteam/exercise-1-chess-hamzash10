//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include "Piece.h"
using namespace std;


const char LIMIT_UP='A';
const char LIMIT_DOWN='H';
const char LIMIT_LEFT='1';
const char LIMIT_RIGHT='8';

class Board {
    array<array<shared_ptr<Piece>, 8>, 8> _board;
    Location white_king_location, black_king_location;
    string current_player;


    //functions
    int check_locations(const string &input);

    int update_locations(const string &input);

    bool will_cause_check() const;

public:
    explicit Board(const string &board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");

    int move(const string &input);
};


#endif //CHESS_BOARD_H
