//
// Created by 2022 on 6/8/2024.
//

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <set>
#include "Board.h"
#include "Rook.h"
#include "King.h"
#include "Empty.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"
#include "Pawn.h"


//TODO: Bonus castling && en passant && pawn promotion
Board::Board(const string &board):current_player(White) {
    for (int i = 0; i < board.size(); ++i) {
        int y=i/8;
        int x=i%8;
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
            case 'Q':
                _board[y][x]= make_shared<Queen>(White,Location(y,x));
                break;
            case 'q':
                _board[y][x]= make_shared<Queen>(Black,Location(y,x));
                break;
            case 'N':
                _board[y][x]= make_shared<Knight>(White,Location(y,x));
                break;
            case 'n':
                _board[y][x]= make_shared<Knight>(Black,Location(y,x));
                break;
            case 'P':
                _board[y][x]= make_shared<Pawn>(White,Location(y,x));
                break;
            case 'p':
                _board[y][x]= make_shared<Pawn>(Black,Location(y,x));
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
    //for pawns special check if the pawn is attacking
    string pawns="Pp";
    if(pawns.find(_board[current.y][current.x]->get_type()) != string::npos) {
        shared_ptr<Pawn> pawn = dynamic_pointer_cast<Pawn>(_board[current.y][current.x]);
        if(!pawn->is_legal_move(_board[destination.y][destination.x]))
            return 21;
    }
    else if (!_board[current.y][current.x]->is_legal_move(destination))
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
    string knights="N";
    string pawns="P";
    shared_ptr<Piece> current_king=(current_player==White)? white_king:black_king;
    if(current_player==White){
        to_lower(straight);
        to_lower(diagonal);
        to_lower(knights);
        to_lower(pawns);
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


    // check north-west the king
    // pawn check only for the black king
    bool pawn_check=current_king->get_color()==Black;
    for (int y = current_king_location.y-1, x = current_king_location.x-1; y>=0 && x>=0; --y,--x) {
        if(_board[y][x]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else
            break;

    }

    //check north-east the king
    // pawn check only for the black king
    pawn_check=current_king->get_color()==Black;
    for (int y = current_king_location.y-1,x = current_king_location.x+1; y>=0 && x<8; --y,++x) {
        if(_board[y][x]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else
            break;
    }

    //check south-west the king
    // pawn check only for the white king
    pawn_check=current_king->get_color()==White;
    for (int y = current_king_location.y+1,x = current_king_location.x-1; y<8 && x>=0; ++y,--x) {
        if(_board[y][x]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else
            break;
    }

    //check south-east the king
    // pawn check only for the white king
    pawn_check=current_king->get_color()==White;
    for (int y = current_king_location.y+1,x = current_king_location.x+1; y<8 && x<8; ++y,++x) {
        if(_board[y][x]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[y][x]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check for knight checks
    vector<pair<int,int>> possible_locations={
            {-2,-1},{-2,+1},{-1,+2},{+1,+2},
            {+2,+1},{+2,-1},{+1,-2},{-1,-2}
    };

    for(auto pl:possible_locations){
        //check if the move in the board
        if(current_king_location.y+pl.first >=0 && current_king_location.y+pl.first<8 &&
                current_king_location.x+pl.second>=0 && current_king_location.x+pl.second<8)
            //check for an opponent knight
            if (knights.find(
                    _board[current_king_location.y + pl.first][current_king_location.x + pl.second]->get_type()) !=
                string::npos)
                return true;

    }

    return false;
}

void Board::to_lower(string &str) const {
    string tmp = str;
    str.clear();
    for (char ch: tmp) {
        str.push_back(tolower(ch));
    }
}


