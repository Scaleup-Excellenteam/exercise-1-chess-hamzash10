//
// Created by 2022 on 6/8/2024.
//

#include <iostream>
#include <list>
#include "Board.h"
#include "Rook.h"
#include "King.h"
#include "Empty.h"
#include "Bishop.h"

Board::Board(const string &board):current_player(White) {
    for (int i = 0; i < board.size(); ++i) {
        int y=i/8;
        int x=i%8;
        char c=board[i];
        switch (board[i]) {
            case 'R':
                _board[y][x]= make_shared<Rook>(White,Location(y,x));
                break;
            case 'r':
                _board[y][x]= make_shared<Rook>(Black,Location(y,x));
                break;
            case 'K':
                _board[y][x]= make_shared<King>(White,Location(y,x));
                white_king= _board[y][x];
                break;
            case 'k':
                _board[y][x]= make_shared<King>(Black,Location(y,x));
                black_king=_board[y][x];
                break;
            case 'B':
                _board[y][x]= make_shared<Bishop>(White,Location(y,x));
                break;
            case 'b':
                _board[y][x]= make_shared<Bishop>(Black,Location(y,x));
                break;
            case '#':
                _board[y][x]= make_shared<Empty>(NoColor,Location(y,x));
                break;

        }
    }
}

int Board::move(const string &input) {
    //convert to locations
    Location current = {tolower(input[0]) - 'a', input[1] - '1'};
    Location destination = {tolower(input[2]) - 'a', input[3] - '1'};

    int response=check_illegal_moves(current,destination);
    if(!response)
        response= check_legal_moves(current,destination);
    return response;
}

int Board::check_illegal_moves(const Location& current,const Location& destination) {
    //11 - there is not piece at the source
    if (_board[current.y][current.x]->get_type() == '#')
        return 11;

    //12 - the piece in the source is piece of your opponent
    if (_board[current.y][current.x]->get_color() != current_player)
        return 12;

    //13 - there one of your pieces at the destination
    if (_board[destination.y][destination.x]->get_color() == current_player)
        return 13;

    //21 - illegal movement of that piece
    if (!_board[current.y][current.x]->is_legal_move(destination))
        return 21;

    return 0;
}

int Board::check_legal_moves(const Location& current,const Location& destination) {

    _board[current.y][current.x]->move(destination);
    shared_ptr<Piece> destination_piece=_board[destination.y][destination.x];
    _board[destination.y][destination.x]=_board[current.y][current.x];
    _board[current.y][current.x]= make_shared<Empty>(NoColor,current);
    //31 - this movement will cause you checkmate
    if (will_cause_check()) {
        //undo move
        _board[destination.y][destination.x]->move(current);
        _board[current.y][current.x]=_board[destination.y][destination.x];
        _board[destination.y][destination.x]=destination_piece;
        return 31;
    }

    //change player
    current_player = (current_player == White) ? Black : White;
    //41 - the last movement was legal and cause check
    if (will_cause_check())
        return 41;

    //42 - the last movement was legal, next turn
    return 42;
}


bool Board::will_cause_check() const {
    string straight="RQ";
    string diagonal="BQ";
    shared_ptr<Piece> current_king=(current_player==White)? white_king:black_king;
    if(current_player==White){
        to_lower(straight);
        to_lower(diagonal);
    }
    Location current_king_location=current_king->get_location();


    //check above the king
    for (int y = current_king_location.y-1; y >= 0; --y) {
        if(_board[y][current_king_location.x]->get_type()=='#')
            continue;
        else if(straight.find(_board[y][current_king_location.x]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check below the king
    for (int y = current_king_location.y+1; y < 8; ++y) {
        if(_board[y][current_king_location.x]->get_type()=='#')
            continue;
        else if(straight.find(_board[y][current_king_location.x]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check left the king
    for (int x = current_king_location.x-1; x >= 0; --x) {
        if(_board[current_king_location.y][x]->get_type()=='#')
            continue;
        else if(straight.find(_board[current_king_location.y][x]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check right the king
    for (int x = current_king_location.x+1; x < 8; ++x) {
        if(_board[current_king_location.y][x]->get_type()=='#')
            continue;
        else if(straight.find(_board[current_king_location.y][x]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check north-west the king
    //check north-east the king
    //check south-west the king
    //check south-east the king
    //check for knight checks
    return false;
}

void Board::to_lower(string &str) const {
    string tmp = str;
    str.clear();
    for (char ch: tmp) {
        str.push_back(tolower(ch));
    }
}


