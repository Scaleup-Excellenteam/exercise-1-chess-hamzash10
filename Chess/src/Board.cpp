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


//TODO: en passant && pawn promotion
Board::Board(const string &board):current_player(White) {
    for (int i = 0; i < board.size(); ++i) {
        int y= i / BOARD_MAX_PLACE;
        int x= i % BOARD_MAX_PLACE;
        switch (tolower(board[i])) {
            case 'r':
                _board[x][y] = create_piece<Rook>(board[i],Location(x, y));
                break;
            case 'k':
                _board[x][y] = create_piece<King>(board[i],Location(x, y));
                if(islower(board[i]))
                    black_king=_board[x][y];
                else
                    white_king=_board[x][y];
                break;
            case 'b':
                _board[x][y] = create_piece<Bishop>(board[i],Location(x, y));
                break;
            case 'q':
                _board[x][y] = create_piece<Queen>(board[i],Location(x, y));
                break;
            case 'n':
                _board[x][y] = create_piece<Knight>(board[i],Location(x, y));
                break;
            case 'p':
                _board[x][y] = create_piece<Pawn>(board[i],Location(x, y));
                break;
            case '#':
                _board[x][y] = create_piece<Empty>(board[i],Location(x, y));
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
        return NoPieceAtSource;

    //12 - the piece in the source is piece of your opponent
    if (_board[current.x][current.y]->get_color() != current_player)
        return PieceOfOpponentAtSource;

    //13 - there one of your pieces at the destination
    if (_board[destination.x][destination.y]->get_color() == current_player)
        return YourPieceAtDestination;

    //21 - illegal movement of that piece
    //for pawns special check if the pawn is attacking
    string pawns="Pp";
    if(is_this_types(pawns,current.x,current.y)) {
        shared_ptr<Pawn> pawn = dynamic_pointer_cast<Pawn>(_board[current.x][current.y]);
        if(!pawn->is_legal_move(_board[destination.x][destination.y]))
            return IllegalMovementOfPiece;
    }
    else if (!_board[current.x][current.y]->is_legal_move(destination))
        return IllegalMovementOfPiece;

//    if(!is_way_clear(current,destination))
//        return IllegalMovementOfPiece;

    return 0;
}

int Board::check_legal_moves(const Location& current,const Location& destination) {

    //43 - the last move was Castling
    if(will_preform_castling(current,destination)) {
        current_player = (current_player == White) ? Black : White;
        return Castling;
    }


    _board[current.x][current.y]->move(destination);
    shared_ptr<Piece> destination_piece=_board[destination.x][destination.y];
    _board[destination.x][destination.y]=_board[current.x][current.y];
    _board[current.x][current.y]= create_piece<Empty>('#',current);
    //31 - this movement will cause you checkmate
    if (will_cause_check()) {
        //undo move
        _board[destination.x][destination.y]->move(current);
        _board[current.x][current.y]=_board[destination.x][destination.y];
        _board[destination.x][destination.y]=destination_piece;
        return WillCauseCheckmate;
    }

    //change player
    current_player = (current_player == White) ? Black : White;
    //41 - the last movement was legal and cause check
    if (will_cause_check())
        return LegalCheck;


    //44 - the last move was checkmate


    //42 - the last movement was legal, next turn
    return LegalNextTurn;
}


bool Board::will_cause_check() const {
    string straight = "RQ";
    string diagonal = "BQ";
    string knights = "N";
    string pawns = "P";
    shared_ptr<Piece> current_king = (current_player == White) ? white_king : black_king;
    if (current_player == White) {
        to_lower(straight);
        to_lower(diagonal);
        to_lower(knights);
        to_lower(pawns);
    }
    Location current_king_location = current_king->get_location();

    //this will store the piece is in the king's way
    Location piece_in_way_location{};
    vector<pair<int, int>> four_straight_threats = {
            {current_king_location.x, BOARD_MIN_PLACE}, // above the king
            {current_king_location.x, BOARD_MAX_PLACE}, // below the king
            {BOARD_MIN_PLACE,         current_king_location.y}, // left the king
            {BOARD_MAX_PLACE,         current_king_location.y}  // right the king
    };

    for (auto direction: four_straight_threats) {
        piece_in_way_location = is_way_clear(current_king_location, Location(direction.first, direction.second));
        if (piece_in_way_location != CLEAR && is_this_types(straight,piece_in_way_location.x,piece_in_way_location.y))
            return true;
    }

    //this helper lambda function will check the diagonal directions
    auto check_diagonal_threat = [&](int x_dir, int y_dir, bool check_pawn) {
        int x = current_king_location.x;
        int y = current_king_location.y;
        while (x >= BOARD_MIN_PLACE && x <= BOARD_MAX_PLACE && y >= BOARD_MIN_PLACE && y <= BOARD_MAX_PLACE) {
            x += x_dir;
            y += y_dir;
        }
        piece_in_way_location = is_way_clear(current_king_location, Location(x, y));
        // pawn check only for the black king
        if (piece_in_way_location != CLEAR && is_this_types(diagonal,piece_in_way_location.x,piece_in_way_location.y))
            return true;
        else if (piece_in_way_location != CLEAR && check_pawn && is_this_types(pawns,piece_in_way_location.x,piece_in_way_location.y))
            return true;
        return false;
    };

    // check north-west the king
    if (check_diagonal_threat(-1, -1,(current_king->get_color() == Black) &&
                                       (piece_in_way_location.x == current_king_location.x - 1) &&
                                       (piece_in_way_location.y == current_king_location.y - 1))) return true;
    //check north-east the king
    if (check_diagonal_threat(+1, -1, (current_king->get_color() == Black) &&
                                      (piece_in_way_location.x == current_king_location.x + 1) &&
                                      (piece_in_way_location.y == current_king_location.y - 1))) return true;
    //check south-west the king
    if (check_diagonal_threat(-1, +1, (current_king->get_color() == White) &&
                                      (piece_in_way_location.x == current_king_location.x - 1) &&
                                      (piece_in_way_location.y == current_king_location.y + 1))) return true;
    //check south-east the king
    if (check_diagonal_threat(+1, +1, (current_king->get_color() == White) &&
                                      (piece_in_way_location.x == current_king_location.x + 1) &&
                                      (piece_in_way_location.y == current_king_location.y + 1))) return true;

    //check for knight checks
    vector<pair<int, int>> possible_locations = {
            {-2, -1},
            {-2, +1},
            {-1, +2},
            {+1, +2},
            {+2, +1},
            {+2, -1},
            {+1, -2},
            {-1, -2}
    };

    for (auto pl: possible_locations) {
        //check if the move in the board
        if (current_king_location.y + pl.first >= BOARD_MIN_PLACE &&
            current_king_location.y + pl.first < BOARD_MAX_PLACE &&
            current_king_location.x + pl.second >= BOARD_MIN_PLACE &&
            current_king_location.x + pl.second < BOARD_MAX_PLACE)
            //check for an opponent knight
            if (is_this_types(knights,current_king_location.x + pl.second,current_king_location.y + pl.first))
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
    return (ch=='#')? NoColor:islower(ch)? Black:White;
}

template<class PieceType>
shared_ptr<PieceType> Board::create_piece(const char &ch, Location starting_location) {
    return make_shared<PieceType>(get_player_color(ch),starting_location);
}

bool Board::will_preform_castling(const Location &current, const Location &destination) {
    /*
     * castling is legal only if:
     * the king and the castle didn't move before
     * the king isn't in check
     * the king won't be in the check after the castling
     * the square between the king starting location and destination isn't threatened by any piece
     */

    string kings = "Kk";
    string rooks="Rr";
    // is the piece not a King?
    if(!is_this_types(kings,current.x,current.y))
        return false;

    // check in what direction the castling is and if the king move two steps
    int x_direction=destination.x-current.x;
    int y_direction=destination.y-current.y;
    string direction;//make enum
    int rook_x_place;
    if(x_direction==2 && y_direction==0) {
        direction = "right";
        rook_x_place = BOARD_MAX_PLACE-1;
    }
    else if(x_direction==-2 && y_direction==0) {
        direction = "left";
        rook_x_place = BOARD_MIN_PLACE;
    }
    else
        return false;

    //check if there is not a rook in the moving direction
    if (!is_this_types(rooks,rook_x_place,destination.y))
        return false;

    // check if there is no piece between the king and the rook (don't include the rook's location)
    if(is_way_clear(current,Location((rook_x_place==BOARD_MIN_PLACE)? rook_x_place+1:rook_x_place-1,destination.y))!=CLEAR)
        return false;

    //check if this is the first move of the king and the rook
    shared_ptr<Piece> current_king=(current_player==White)? white_king:black_king;
    shared_ptr<Piece> current_rook=_board[rook_x_place][destination.y];
    if(!dynamic_pointer_cast<King>(current_king)->is_first_move() || !dynamic_pointer_cast<Rook>(current_rook)->is_first_move())
        return false;

    //check if the square between the king's location and destination is clear
    Location square_between{};
    square_between.y=current.y;
    if(direction=="right")
        square_between.x=current.x+1;
    else
        square_between.x=current.x-1;

    // move the king to the square between piece and check if there is a check
    _board[current.x][current.y]->move(square_between);
    shared_ptr<Piece> destination_piece=_board[square_between.x][square_between.y];
    _board[square_between.x][square_between.y]=_board[current.x][current.y];
    _board[current.x][current.y]= destination_piece;
    if (will_cause_check()) {
        //undo move
        _board[square_between.x][square_between.y]->move(current);
        _board[current.x][current.y]=_board[square_between.x][square_between.y];
        _board[square_between.x][square_between.y]=destination_piece;
        return false;
    }
    //move king to destination
    destination_piece = _board[destination.x][destination.y];
    _board[square_between.x][square_between.y]->move(destination);
    _board[destination.x][destination.y]=_board[square_between.x][square_between.y];
    _board[square_between.x][square_between.y]=destination_piece;
    if (will_cause_check()) {
        //undo move
        destination_piece = _board[current.x][current.y];
        _board[destination.x][destination.y]->move(current);
        _board[current.x][current.y]=_board[destination.x][destination.y];
        _board[destination.x][destination.y]=destination_piece;
        return false;
    }
    //move the rook to the king's side
    destination_piece=_board[square_between.x][square_between.y];
    Location rook_original_location=current_rook->get_location();
    _board[current_rook->get_location().x][current_rook->get_location().y]->move(square_between);
    _board[square_between.x][square_between.y]=_board[rook_original_location.x][rook_original_location.y];
    _board[rook_original_location.x][rook_original_location.y]= destination_piece;
    return true;
}

Location Board::is_way_clear(const Location &current, const Location &destination) const {
    /*
     * checks if there is no piece between the current location and destination
     * returns CLEAR if there is nothing
     * returns the location of the piece if there is
     */

    //positive means right, negative means left
    int deltaX = destination.x - current.x;
    //positive means down, negative means up
    int deltaY = destination.y - current.y;

    //moving left or right
    if (deltaY == 0) {
        //moving right
        for (int x = current.x + 1; x <= destination.x; ++x) {
            if(_board[x][current.y]->get_type()=='#')
                continue;
            return Location(x,current.y);
        }
        //moving left
        for (int x = current.x - 1; x >= destination.x; --x) {
            if(_board[x][current.y]->get_type()=='#')
                continue;
            return Location(x,current.y);
        }
    }
    //moving up  or down
    else if (deltaX == 0) {
        //moving down
        for (int y = current.y + 1; y <= destination.y; ++y) {
            if(_board[current.x][y]->get_type()=='#')
                continue;
            return Location(current.x,y);
        }
        //moving up
        for (int y = current.y - 1; y >= destination.y; --y) {
            if(_board[current.x][y]->get_type()=='#')
                continue;
            return Location(current.x,y);
        }
    }
    //moving diagonally
    else if(deltaX<0 && deltaY<0) {
        // check north-west
        for (int y = current.y - 1, x = current.x - 1; y >= BOARD_MIN_PLACE && x >= BOARD_MIN_PLACE; --y, --x) {
            if (_board[x][y]->get_type() == '#')
                continue;
            return Location(x, y);
        }
    }else if(deltaX>0 && deltaY<0) {
        //check north-east
        for (int y = current.y - 1, x = current.x + 1; y >= BOARD_MIN_PLACE && x < BOARD_MAX_PLACE; --y, ++x) {
            if (_board[x][y]->get_type() == '#')
                continue;
            return Location(x, y);
        }
    }
    else if(deltaX<0 && deltaY>0) {
        //check south-west
        for (int y = current.y + 1, x = current.x - 1; y < BOARD_MAX_PLACE && x >= BOARD_MIN_PLACE; ++y, --x) {
            if (_board[x][y]->get_type() == '#')
                continue;
            return Location(x, y);
        }
    }
    else if(deltaX>0 && deltaY>0) {
        //check south-east
        for (int y = current.y + 1, x = current.x + 1; y < BOARD_MAX_PLACE && x < BOARD_MAX_PLACE; ++y, ++x) {
            if (_board[x][y]->get_type() == '#')
                continue;
            return Location(x, y);
        }
    }
    return CLEAR;
}

bool Board::is_this_types(const string &types, const int &x, const int &y) const {
    return types.find(_board[x][y]->get_type()) != string::npos;
}



