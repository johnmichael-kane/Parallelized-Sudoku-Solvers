/*
 * Pos.cpp
 *
 *  Created on: 14 Jul 2015
 *      Author: eva
 *  Updated on: 8 Mar 2024
 * 		Editor: Soleil Cordray
 */

#include "Pos.h"

// Constructor definition for Pos.
// Initializes the Pos object with the specified row and column values.
Pos::Pos(int row, int col) : row(row), col(col)
{
}

// Returns a string representation of the Pos object.
string Pos::toString()
{
	stringstream ss;
	ss << "[";
	ss << row;
	ss << ",";
	ss << col;
	ss << "]";
	return ss.str();
}
