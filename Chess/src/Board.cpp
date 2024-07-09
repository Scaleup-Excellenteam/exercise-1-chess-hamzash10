//
// Created by 2022 on 6/8/2024.
//

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <set>
#include "../include/Board.h"
#include "../include/Rook.h"
#include "../include/King.h"
#include "../include/Empty.h"
#include "../include/Bishop.h"
#include "../include/Queen.h"
#include "../include/Knight.h"
#include "../include/Pawn.h"


//TODO: Bonus castling && en passant && pawn promotion
Board::Board(const string &board):current_player(White) {
    for (int i = 0; i < board.size(); ++i) {
        int y=i/8;
        int x=i%8;
        switch (tolower(board[i])) {
            case 'r':
                _board[x][y]= make_shared<Rook>(get_player_color(board[i]),Location(x, y));
                break;
            case 'k':
                _board[x][y]= make_shared<King>(get_player_color(board[i]),Location(x, y));
                black_king=_board[x][y];
                break;
            case 'b':
                _board[x][y]= make_shared<Bishop>(get_player_color(board[i]),Location(x, y));
                break;
            case 'q':
                _board[x][y]= make_shared<Queen>(get_player_color(board[i]),Location(x, y));
                break;
            case 'n':
                _board[x][y]= make_shared<Knight>(get_player_color(board[i]),Location(x, y));
                break;
            case 'p':
                _board[x][y]= make_shared<Pawn>(get_player_color(board[i]),Location(x, y));
                break;
            case '#':
                _board[x][y]= make_shared<Empty>(NoColor,Location(x, y));
                break;


        }
    }
}

int Board::move(const string &input) {
    //convert to locations
    Location current = {input[1] - '1',tolower(input[0]) - 'a'};
    Location destination = {input[3] - '1', tolower(input[2]) - 'a' };

    int response=check_illegal_moves(current,destination);
    if(!response)
        response= check_legal_moves(current,destination);
    return response;
}

int Board::check_illegal_moves(const Location& current,const Location& destination) {
    //11 - there is not piece at the source
    if (_board[current.x][current.y]->get_type() == '#')
        return 11;

    //12 - the piece in the source is piece of your opponent
    if (_board[current.x][current.y]->get_color() != current_player)
        return 12;

    //13 - there one of your pieces at the destination
    if (_board[destination.x][destination.y]->get_color() == current_player)
        return 13;

    //21 - illegal movement of that piece
    //for pawns special check if the pawn is attacking
    string pawns="Pp";
    if(pawns.find(_board[current.x][current.y]->get_type()) != string::npos) {
        shared_ptr<Pawn> pawn = dynamic_pointer_cast<Pawn>(_board[current.x][current.y]);
        if(!pawn->is_legal_move(_board[destination.x][destination.y]))
            return 21;
    }
    else if (!_board[current.x][current.y]->is_legal_move(destination))
        return 21;

    return 0;
}

int Board::check_legal_moves(const Location& current,const Location& destination) {

    _board[current.x][current.y]->move(destination);
    shared_ptr<Piece> destination_piece=_board[destination.x][destination.y];
    _board[destination.x][destination.y]=_board[current.x][current.y];
    _board[current.x][current.y]= make_shared<Empty>(NoColor,current);
    //31 - this movement will cause you checkmate
    if (will_cause_check()) {
        //undo move
        _board[destination.x][destination.y]->move(current);
        _board[current.x][current.y]=_board[destination.x][destination.y];
        _board[destination.x][destination.y]=destination_piece;
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
        if(_board[current_king_location.x][y]->get_type()=='#')
            continue;
        else if(straight.find(_board[current_king_location.x][y]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check below the king
    for (int y = current_king_location.y+1; y < 8; ++y) {
        if(_board[current_king_location.x][y]->get_type()=='#')
            continue;
        else if(straight.find(_board[current_king_location.x][y]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check left the king
    for (int x = current_king_location.x-1; x >= 0; --x) {
        if(_board[x][current_king_location.y]->get_type()=='#')
            continue;
        else if(straight.find(_board[x][current_king_location.y]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    //check right the king
    for (int x = current_king_location.x+1; x < 8; ++x) {
        if(_board[x][current_king_location.y]->get_type()=='#')
            continue;
        else if(straight.find(_board[x][current_king_location.y]->get_type()) != string::npos)
            return true;
        else
            break;
    }


    // check north-west the king
    // pawn check only for the black king
    bool pawn_check=current_king->get_color()==Black;
    for (int y = current_king_location.y-1, x = current_king_location.x-1; y>=0 && x>=0; --y,--x) {
        if(_board[x][y]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[x][y]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[x][y]->get_type()) != string::npos)
            return true;
        else
            break;

    }

    //check north-east the king
    // pawn check only for the black king
    pawn_check=current_king->get_color()==Black;
    for (int y = current_king_location.y-1,x = current_king_location.x+1; y>=0 && x<8; --y,++x) {
        if(_board[x][y]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[x][y]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[x][y]->get_type()) != string::npos)
            return true;
        else
            break;
    }

    //check south-west the king
    // pawn check only for the white king
    pawn_check=current_king->get_color()==White;
    for (int y = current_king_location.y+1,x = current_king_location.x-1; y<8 && x>=0; ++y,--x) {
        if(_board[x][y]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[x][y]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[x][y]->get_type()) != string::npos)
            return true;
        else
            break;
    }

    //check south-east the king
    // pawn check only for the white king
    pawn_check=current_king->get_color()==White;
    for (int y = current_king_location.y+1,x = current_king_location.x+1; y<8 && x<8; ++y,++x) {
        if(_board[x][y]->get_type()=='#') {
            pawn_check = false;
            continue;
        }else if(diagonal.find(_board[x][y]->get_type()) != string::npos)
            return true;
        else if(pawn_check && pawns.find(_board[x][y]->get_type()) != string::npos)
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
                    _board[current_king_location.x + pl.second][current_king_location.y + pl.first]->get_type()) !=
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

Player Board::get_player_color(const char &ch) {
    return islower(ch)? Black:White;
}



