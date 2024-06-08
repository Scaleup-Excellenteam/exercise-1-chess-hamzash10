//
// Created by 2022 on 6/8/2024.
//


#include <cmath>
#include "Bishop.h"

Bishop::Bishop(const Player &color, const Location &location):Piece('b',color,location) {

}

void Bishop::move(const Location &destination) {
    _location.y=destination.y;
    _location.x=destination.x;
}

bool Bishop::is_legal_move(const Location &destination) {
    // since the bishop moves diagonally, I can calculate the slope of the line connecting the current location and the destination,
    // and calculate the angel of it and check if the angel%90==45
    // am I proud of my self?
    // yes, yes I'm
    double deltaX=destination.x-_location.x;
    double deltaY=destination.y-_location.y;
    double angle_in_radians = atan2(deltaY, deltaX);
    double angle_in_degrees = angle_in_radians * 180.0 / std::numbers::pi;
    //i checked epsilon because sometimes the calculation precise
    return (abs(abs(angle_in_degrees) - 45 )< 0.0001 ||
            abs(abs(angle_in_degrees) - 135) < 0.0001 ||
            abs(abs(angle_in_degrees) - 225) < 0.0001 ||
            abs(abs(angle_in_degrees) - 315) < 0.0001);
}
