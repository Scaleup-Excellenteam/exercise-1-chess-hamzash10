//
// Created by 2022 on 6/8/2024.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <memory>
#include <vector>

using namespace std;

const int BOARD_MAX_PLACE = 8;
const int BOARD_MIN_PLACE = 0;

struct Location{
    int x;
    int y;
    bool operator!=(const Location& rhs) {
        return x!=rhs.x | y!=rhs.y;
    }
    bool operator==(const Location& other) const {
        return x == other.x && y == other.y;
    }
};

ostream& operator<<(std::ostream& os, const Location& loc);


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

    virtual vector<shared_ptr<Location>> all_possible_moves()=0;
};

#endif //CHESS_PIECE_H
