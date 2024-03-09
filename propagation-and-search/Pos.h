/*
 * Pos.h
 *
 *  Created on: 14 Jul 2015
 *      Author: eva
 *  Updated on: 8 Mar 2024
 *      Editor: Soleil Cordray
 */

#ifndef POS_H_
#define POS_H_

#include <sstream>
using namespace std;

struct Pos
{
	int row, col;

	// Constructor directly in the header file.
	Pos(int r, int c) : row(r), col(c) {}

	// toString method directly in the header file.
	string toString() const
	{
		stringstream ss;
		ss << "[" << row << ", " << col << "]";
		return ss.str();
	}
};

#endif /* POS_H_ */
