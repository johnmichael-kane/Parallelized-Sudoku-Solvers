// Name        : Grid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef POSITION_H_
#define POSITION_H_

#include <sstream>

using namespace std;

struct Position {
    int row, col;

    Position(int r, int c) : row(r), col(c) {}

    string toString() const {
        stringstream ss;
        ss << "[" << row << ", " << col << "]";
        return ss.str();
    }
};

#endif /* POSITION_H_ */