//
// Created by 2022 on 6/8/2024.
//

#include <iostream>
#include "Board.h"
#include "Rook.h"
#include "King.h"
#include "Empty.h"

Board::Board(const string &board):current_player("white") {
    for (int i = 0; i < board.size(); ++i) {
        int y=i/8;
        int x=i%8;
        char c=board[i];
        switch (board[i]) {
            case 'R':
                _board[y][x]= make_shared<Rook>("white",Location(y,x));
                break;
            case 'r':
                _board[y][x]= make_shared<Rook>("black",Location(y,x));
                break;
            case 'K':
                _board[y][x]= make_shared<King>("white",Location(y,x));
                white_king_location=Location(y,x);
                break;
            case 'k':
                _board[y][x]= make_shared<King>("black",Location(y,x));
                black_king_location=Location(y,x);
                break;
            case '#':
                _board[y][x]= make_shared<Empty>("empty",Location(y,x));
                break;

        }
    }
}

int Board::move(const string &input) {
    int response=update_locations(input);

    return response;
}

int Board::update_locations(const string &input) {
    int response=check_locations(input);
    return response;
}

int Board::check_locations(const string &input) {

    //convert to locations
    int currentY=tolower(input[0])-'a';
    int currentX=input[1]-'1';
    int destinationY=tolower(input[2])-'a';
    int destinationX=input[3]-'1';

    //11 - there is not piece at the source
    if(_board[currentY][currentX]->get_type()=='#')
        return 11;

    //12 - the piece in the source is piece of your opponent
    if(_board[currentY][currentX]->get_color()!=current_player)
        return 12;

    //13 - there one of your pieces at the destination
    if(_board[destinationY][destinationX]->get_color()==current_player)
        return 13;

    //31 - this movement will cause you checkmate
    if(will_cause_check())
        return 31;

    //21 - illegal movement of that piece
    //41 - the last movement was legal and cause check
    //42 - the last movement was legal, next turn
    int response=_board[currentY][currentX]->move(Location(destinationY,destinationX));

    current_player=(current_player=="white")? "black":"white";

    return response;
}

bool Board::will_cause_check() const {
    //check above the king
    //check below the king
    //check left the king
    //check right the king
    //check north-west the king
    //check north-east the king
    //check south-west the king
    //check south-east the king
    //check for knight checks
    return false;
}


