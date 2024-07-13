//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include "Piece.h"
#include "../include/PriorityQueue.h"
using namespace std;


const Location CLEAR(-1,-1);
const int DEPTH=2;
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

enum Values{
    PawnValue=1,
    KnightValue=3,
    BishopValue=3,
    RookValue=5,
    QueenValue=9,
    KingValue=100,
    Threatened=-2, // threatened by weaker piece
    Threatening=2 // threatening a stronger piece
};


class Board {
    array<array<shared_ptr<Piece>, 8>, 8> _board;
    shared_ptr<Piece> white_king, black_king;
    Player current_player;
    PriorityQueue<pair<vector<pair<Location,Location>>,int>> best_moves;

    //functions
    int check_illegal_moves(const Location& current,const Location& destination) const;
    int check_legal_moves(const Location& current,const Location& destination);
    vector<Location> is_piece_threatened(const Location& location) const;
    shared_ptr<Piece> get_current_king() const;
    void to_lower(string& str) const;
    Player get_player_color(const char& ch);
    template<class PieceType>
    shared_ptr<PieceType> create_piece(const char& ch, Location starting_location);
    bool will_preform_castling(const Location& current,const Location& destination);
    Location is_way_clear(const Location& current,const Location& destination) const;
    bool is_this_types(const string& types,const int& x,const int& y) const;
    bool will_cause_checkmate();
    void change_places(const Location& current,const Location& destination,shared_ptr<Piece> destination_piece= nullptr);
    vector<shared_ptr<Piece>> get_all_pieces_of_player(Player player) const;
    pair<vector<pair<Location,Location>>,int> calculate_values(Board board, Player current_player, const int& depth);
    int threatened_by_weaker_piece(const Location &location);
    int threatening_stronger_piece(const Location &location);
    int get_piece_value( const Location& location) const;
public:
    explicit Board(const string &board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");
    int move(const string &input);
    friend ostream& operator<<(ostream& os, Board& board);
};

#endif //CHESS_BOARD_H
