// Name        : Grid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef POSITION_H_
#define POSITION_H_

#include <sstream>

struct Position {
    int row, col;

    Position(int r, int c) : row(r), col(c) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "[" << row << ", " << col << "]";
        return ss.str();
    }
};

#endif /* POSITION_H_ */