//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include "Piece.h"
using namespace std;



class Board {
    array<array<shared_ptr<Piece>, 8>, 8> _board;
    shared_ptr<Piece> white_king, black_king;
    Player current_player;


    //functions
    int check_locations(const string &input);

    int check_illegal_moves(const Location& current,const Location& destination);
    int check_legal_moves(const Location& current,const Location& destination);
    bool will_cause_check() const;

    void to_lower(string& str) const;

public:
    explicit Board(const string &board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");

    int move(const string &input);
};


#endif //CHESS_BOARD_H
