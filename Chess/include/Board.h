//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include "Piece.h"
using namespace std;

const int BOARD_MAX_PLACE = 8;
const int BOARD_MIN_PLACE = 8;

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
    Player get_player_color(const char& ch);
    template<class PieceType>
    shared_ptr<PieceType> create_piece(const char& ch, Location starting_location);
public:
    explicit Board(const string &board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");

    int move(const string &input);
};


#endif //CHESS_BOARD_H
