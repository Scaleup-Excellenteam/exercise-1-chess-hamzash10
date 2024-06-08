//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <memory>
using namespace std;


struct Location{
    int y,x;
};

class Piece {
    char piece_type;
protected:
    Location _location;
    const string color;

    virtual bool is_legal_move(const Location& destination) = 0;

public:
    Piece(const char &piece_type, const string &color, const Location &location);

    virtual ~Piece() = default;

    Location get_location();

    char get_type() const;

    string get_color() const;

    virtual int move(const Location &destination) = 0;
};

#endif //CHESS_PIECE_H
