//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include "Piece.h"
using namespace std;


const Location CLEAR(-1,-1);
enum Response{
    // Illegal movements
    NoPieceAtSource = 11,
    PieceOfOpponentAtSource = 12,
    YourPieceAtDestination = 13,
    IllegalMovementOfPiece = 21,
    WillCauseCheckmate = 31,

    // Legal movements
    LegalCheck = 41,
    LegalNextTurn = 42,
    Castling = 43,
    Checkmate = 44
};
class Board {
    array<array<shared_ptr<Piece>, 8>, 8> _board;
    shared_ptr<Piece> white_king, black_king;
    Player current_player;


    //functions
    int check_illegal_moves(const Location& current,const Location& destination);
    int check_legal_moves(const Location& current,const Location& destination);
    bool will_cause_check() const;

    void to_lower(string& str) const;
    Player get_player_color(const char& ch);
    template<class PieceType>
    shared_ptr<PieceType> create_piece(const char& ch, Location starting_location);
    bool will_preform_castling(const Location& current,const Location& destination);
    Location is_way_clear(const Location& current,const Location& destination) const;
    bool is_this_types(const string& types,const int& x,const int& y) const;
    bool will_cause_checkmate();
    void change_places(const Location& current,const Location& destination,shared_ptr<Piece> destination_piece= nullptr);
public:
    explicit Board(const string &board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");

    int move(const string &input);
};


#endif //CHESS_BOARD_H
