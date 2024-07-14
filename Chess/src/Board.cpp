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
    int response;
    Location current;
    Location destination;
    //convert to locations

    current = {input[1] - '1', tolower(input[0]) - 'a'};
    destination = {input[3] - '1', tolower(input[2]) - 'a'};
    if(!is_valid_location(current) || !is_valid_location(destination))
        BoardStateException::getInstance("location or destination is not on the board: "+input + "\n");
    response = check_illegal_moves(current, destination);
    if (!response) {
        response = check_legal_moves(current, destination);
        if(response == WillCauseCheckmate)
            IllegalMoveException::getInstance("illegal move, error code: " + to_string(response) + "\n");
    }
    else
        IllegalMoveException::getInstance("illegal move, error code: " + to_string(response) + "\n");

    return response;
}

int Board::check_illegal_moves(const Location& current,const Location& destination) const {
    //11 - there is not piece at the source
    if (_board[current.x][current.y]->get_type() == '#')
        return NoPieceAtSource;

    //12 - the piece in the source is piece of your opponent
    if (_board[current.x][current.y]->get_color() != current_player)
        return PieceOfOpponentAtSource;

    //13 - there one of your pieces at the destination
    if (_board[destination.x][destination.y]->get_color() == current_player)
        return YourPieceAtDestination;

    //before checking if the move is legal, if the king is in check then disable the castling option
    string kings = "Kk";
    // is the piece a King, that is checked?
    if(is_this_types(kings,current.x,current.y) && !is_piece_threatened(get_current_king()->get_location()).empty()){
        //move the king back and forward to disable the castling
        _board[current.x][current.y]->move(destination);
        _board[current.x][current.y]->move(current);
    }

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

    auto piece_in_way_location=is_way_clear(current,destination);
    if(piece_in_way_location!=CLEAR) {
        if(_board[piece_in_way_location.x][piece_in_way_location.y]->get_color()==current_player)
            return IllegalMovementOfPiece;
    }

    return 0;
}

int Board::check_legal_moves(const Location& current,const Location& destination) {

    //43 - the last move was Castling
    if(will_preform_castling(current,destination)) {
        current_player = (current_player == White) ? Black : White;
        return Castling;
    }

    shared_ptr<Piece> destination_piece=_board[destination.x][destination.y];
    change_places(current,destination);
    //31 - this movement will cause you checkmate
    if (!is_piece_threatened(get_current_king()->get_location()).empty()) {
        //undo move
        change_places(destination,current, destination_piece);
        return WillCauseCheckmate;
    }

    //change player
    current_player = (current_player == White) ? Black : White;
    //41 - the last movement was legal and cause check
    if (!is_piece_threatened(get_current_king()->get_location()).empty()) {
        //44 - the last move was checkmate
        if (will_cause_checkmate())
            return Checkmate;
        return LegalCheck;
    }

    //42 - the last movement was legal, next turn
    return LegalNextTurn;
}


vector<Location> Board::is_piece_threatened(const Location& location) const {
    //returns all threatening pieces locations
    string straight = "RQ";
    string diagonal = "BQ";
    string knights = "N";
    string pawns = "P";

    shared_ptr<Piece> current_Piece = _board[location.x][location.y];
    if (current_player == White) {
        to_lower(straight);
        to_lower(diagonal);
        to_lower(knights);
        to_lower(pawns);
    }
    Location current_piece_location = current_Piece->get_location();

    vector<Location> threats;

    //this will store the pieces that is in the way
    Location piece_in_way_location{};
    vector<pair<int, int>> four_straight_threats = {
            {current_piece_location.x, BOARD_MIN_PLACE}, // above the king
            {current_piece_location.x, BOARD_MAX_PLACE-1}, // below the king
            {BOARD_MIN_PLACE,         current_piece_location.y}, // left the king
            {BOARD_MAX_PLACE-1,         current_piece_location.y}  // right the king
    };

    //for each straight direction
    for (auto direction: four_straight_threats) {
        piece_in_way_location = is_way_clear(current_piece_location, Location(direction.first, direction.second));
        if (piece_in_way_location != CLEAR && is_this_types(straight,piece_in_way_location.x,piece_in_way_location.y))
            //return true;
            threats.push_back(piece_in_way_location);
    }

    //this helper lambda function will check the diagonal directions
    auto check_diagonal_threat = [&](int x_dir, int y_dir, bool check_pawn) {
        int x = current_piece_location.x;
        int y = current_piece_location.y;
        while (x >= BOARD_MIN_PLACE && x <= BOARD_MAX_PLACE && y >= BOARD_MIN_PLACE && y <= BOARD_MAX_PLACE) {
            x += x_dir;
            y += y_dir;
        }
        piece_in_way_location = is_way_clear(current_piece_location, Location(x, y));
        // pawn check only for the black king
        if (piece_in_way_location != CLEAR && is_this_types(diagonal,piece_in_way_location.x,piece_in_way_location.y))
            return true;
        else if (piece_in_way_location != CLEAR && check_pawn && is_this_types(pawns,piece_in_way_location.x,piece_in_way_location.y))
            return true;
        return false;
    };

    //TODO make it simpler
    // check north-west the king
    if (check_diagonal_threat(-1, -1,(current_Piece->get_color() == Black) &&
                                       (piece_in_way_location.x == current_piece_location.x - 1) &&
                                       (piece_in_way_location.y == current_piece_location.y - 1)))
        threats.push_back(piece_in_way_location);
    //check north-east the piece
    if (check_diagonal_threat(+1, -1, (current_Piece->get_color() == Black) &&
                                      (piece_in_way_location.x == current_piece_location.x + 1) &&
                                      (piece_in_way_location.y == current_piece_location.y - 1)))
        threats.push_back(piece_in_way_location);
    //check south-west the piece
    if (check_diagonal_threat(-1, +1, (current_Piece->get_color() == White) &&
                                      (piece_in_way_location.x == current_piece_location.x - 1) &&
                                      (piece_in_way_location.y == current_piece_location.y + 1)))
        threats.push_back(piece_in_way_location);
    //check south-east the piece
    if (check_diagonal_threat(+1, +1, (current_Piece->get_color() == White) &&
                                      (piece_in_way_location.x == current_piece_location.x + 1) &&
                                      (piece_in_way_location.y == current_piece_location.y + 1)))
        threats.push_back(piece_in_way_location);

    //check for knights
    vector<Location> possible_locations = {
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
        if (current_piece_location.y + pl.y >= BOARD_MIN_PLACE &&
            current_piece_location.y + pl.y < BOARD_MAX_PLACE &&
            current_piece_location.x + pl.x >= BOARD_MIN_PLACE &&
            current_piece_location.x + pl.x < BOARD_MAX_PLACE)
            //check for an opponent knight
            if (is_this_types(knights,current_piece_location.x + pl.x,current_piece_location.y + pl.y))
                threats.push_back({current_piece_location.x + pl.x, current_piece_location.y + pl.y});

    }

    return threats;
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
    shared_ptr<Piece> current_king=get_current_king();
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
    shared_ptr<Piece> destination_piece=_board[square_between.x][square_between.y];
    change_places(current,square_between);
    if (!is_piece_threatened(get_current_king()->get_location()).empty()) {
        //undo move
        change_places(square_between,current,destination_piece);
        return false;
    }
    //move king to destination
    destination_piece=_board[destination.x][destination.y];
    change_places(square_between,destination);
    if (!is_piece_threatened(get_current_king()->get_location()).empty()) {
        //move the king back to the starting position
        change_places(destination,current,destination_piece);
        return false;
    }
    //move the rook to the king's side
    change_places(current_rook->get_location(),square_between);
    return true;
}

Location Board::is_way_clear(const Location &current, const Location &destination) const {
    /*
     * checks if there is no piece between the current location and destination
     * returns CLEAR if there is nothing
     * returns the location of the piece if there is
     */

    //knights can jump over pieces so this logic doesn't apply on them
    if(is_this_types("Nn",current.x,current.y))
        return CLEAR;

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

bool Board::will_cause_checkmate() {
    /*
     * this will run only if the opponent king is in check
     * getting all the possible moves for each piece and checking if there is one that can save the king by blocking the check
     */

    //get all the pieces
    vector<shared_ptr<Piece>> threatened_king_pieces=get_all_pieces_of_player(current_player);

    for(auto piece: threatened_king_pieces){
        vector<shared_ptr<Location>> possible_moves = piece->all_possible_moves();
        for(auto move : possible_moves){
            int response = check_illegal_moves(piece->get_location(),*move);
            if(!response) {
                auto starting_location=piece->get_location();
                response = check_legal_moves(piece->get_location(), *move);
                if(response==LegalNextTurn || response==LegalCheck || response== Checkmate){
                    /*
                     * LegalNextTurn: this move clears the check.
                     * LegalCheck: this move clears the check and check the opponent king back.
                     * Checkmate: this move clears the check and checkmates the opponent king.
                     */
                    //undo move
                    change_places(*move,starting_location);
                    current_player = (current_player == White) ? Black : White;
                    return false;
                }
            }
        }
    }
    return true;
}

void Board::change_places(const Location &current, const Location &destination,shared_ptr<Piece> destination_piece) {
    _board[current.x][current.y]->move(destination);
    _board[destination.x][destination.y]=_board[current.x][current.y];
    if(destination_piece != nullptr)
        _board[current.x][current.y]= destination_piece;
    else
        _board[current.x][current.y] = create_piece<Empty>('#', current);

}


vector<shared_ptr<Piece>> Board::get_all_pieces_of_player(Player player) const {
    vector<shared_ptr<Piece>> all_pieces;
    for (int x = 0; x < BOARD_MAX_PLACE; ++x)
        for (int y = 0; y < BOARD_MAX_PLACE; ++y)
            if (_board[x][y]->get_color() == player)
                all_pieces.push_back(_board[x][y]);
    return all_pieces;
}


// ----------Algorithm--------------
pair<vector<pair<Location,Location>>,int> Board::calculate_values(Board board, Player current_player_color, const int &depth) {
    if(depth<0)
        return {vector<pair<Location,Location>>({{CLEAR,CLEAR}}),0};

    //get all the pieces
    vector<shared_ptr<Piece>> player_pieces = board.get_all_pieces_of_player(current_player_color);
    //setting the current player color for the bord
    board.current_player=current_player_color;
    // opponent
    Player opponent_player= (current_player_color==White)? Black:White;

    vector<pair<Location,Location>> best_route;
    int best_route_value = INT_MIN;

    //iterate through the pieces
    for(auto piece:player_pieces){
        // add the starting location to the beginning of the route
        auto starting_location = piece->get_location();
        //iterate through the moves of each piece
        auto all_moves=piece->all_possible_moves();
        for(auto move:all_moves) {
            auto destination=*move;
            //check if the move is legal
            int response = board.check_illegal_moves(piece->get_location(), destination);
            if (!response) {

                // if the move eats a piece get the value of that piece
                int eaten_piece_value= get_piece_value(destination);

                //simulate the move
                response = board.check_legal_moves(piece->get_location(), destination);
                if(response==LegalNextTurn || response==LegalCheck || response== Checkmate) {
                    int current_route_value = 0;
                    vector<pair<Location,Location>> current_route = {{starting_location, destination}};
                    //add the value of the move
                    //the function check_legal_moves() changes the player color, so I reversed it
                    board.current_player=(board.current_player == White)? Black:White;
                    current_route_value += board.threatened_by_weaker_piece(destination);
                    current_route_value += board.threatening_stronger_piece(destination);
                    current_route_value += eaten_piece_value;

                    //checking the opponent move
                    board.current_player=(board.current_player == White)? Black:White;
                    //subtract the opponent counter move value
                    auto rest_route=calculate_values(board,opponent_player,depth-1);
                    board.current_player=(board.current_player == White)? Black:White;
                    current_route_value -= rest_route.second;
                    //undo move
                    board.change_places(destination, starting_location);

                    int good_route= 0;
                    auto it=rest_route.first.begin();
                    while(it!=rest_route.first.end()){
                        if(it->first==CLEAR || it->second==CLEAR) {
                            good_route = 1;
                            break;
                        }
                        it++;
                    }
                    current_route.insert(current_route.end(),rest_route.first.begin(),rest_route.first.end()-good_route);
                    if (current_route_value >= best_route_value) {
                        best_route = current_route;
                        best_route_value = current_route_value;
                        best_moves.push({best_route,best_route_value});
                    }
                }
            }
        }
    }
    return {best_route, best_route_value};
}

int Board::threatened_by_weaker_piece(const Location &location) {
    //check if the piece is threatened by a weaker opponent's piece
    auto current_piece_value=get_piece_value(location);
    //get all the pieces that is threatening the current piece
    auto threatening_pieces= is_piece_threatened(location);
    //check if any threatening piece value is lower than the current piece
    for(auto threat_location:threatening_pieces)
        if (current_piece_value > get_piece_value(threat_location))
            return -current_piece_value;
    return 0;
}

int Board::threatening_stronger_piece(const Location &location) {
    /*
     * the function runs through all the opponent pieces and using previous functions checks if the piece is threatened by the move
     */
    auto current_piece_value=get_piece_value(location);
    auto player=(current_player==White)? Black:White;
    auto opponent=(player==White)? Black:White;
    current_player=player;
    //get all the pieces of the opponent
    auto opponent_pieces=get_all_pieces_of_player(opponent);
    for(auto piece:opponent_pieces){
        //check if the opponent piece is threatened by the move
        current_player=opponent;
        auto threats=is_piece_threatened(piece->get_location());
        current_player=player;
        auto it= find(threats.begin(),threats.end(),location);
        if(it != threats.end()) {
            if(current_piece_value < get_piece_value(*it)) {
                current_player=opponent;
                return get_piece_value(*it);
            }
        }
    }
    current_player=opponent;
    return 0;
}

int Board::get_piece_value(const Location &location) const {
    char piece = _board[location.x][location.y]->get_type();
    switch(tolower(piece)){
        case '#':
            return 0;
        case 'k':
            return KingValue;
        case 'q':
            return QueenValue;
        case 'r':
            return RookValue;
        case 'n':
            return KnightValue;
        case 'b':
            return BishopValue;
        case 'p':
            return PawnValue;

    }
}

shared_ptr<Piece> Board::get_current_king() const {
    return (current_player==White)? white_king:black_king;
}


ostream& operator<<(ostream& os,Board& board) {
    board.calculate_values(board,board.current_player,2);
    if (!board.best_moves.empty()) {
        auto best_move = board.best_moves.pull().first;
        os << "Recommended move: " << best_move.front().first;
        best_move.pop_back();
        os << " " << best_move.front().second;
    } else {
        os << "No recommended move available.";
    }
    board.best_moves.clear();
    return os;
}

bool Board::is_valid_location(const Location& location) {
    return location.x >= BOARD_MIN_PLACE && location.x < BOARD_MAX_PLACE && location.y >= BOARD_MIN_PLACE && location.y < BOARD_MAX_PLACE;
}
