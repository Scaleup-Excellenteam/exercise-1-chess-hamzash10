//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <memory>
using namespace std;


struct Location{
    int x;
    int y;
};

enum Player{
    White,
    Black,
    NoColor
};

class Piece {
    char piece_type;
protected:
    Location _location;
    const Player color;

public:
    Piece(const char &piece_type, const Player &color, const Location &location);

    virtual ~Piece() = default;

    Location get_location();

    char get_type() const;

    Player get_color() const;

    virtual void move(const Location &destination) = 0;

    virtual bool is_legal_move(const Location& destination) = 0;
};

#endif //CHESS_PIECE_H
