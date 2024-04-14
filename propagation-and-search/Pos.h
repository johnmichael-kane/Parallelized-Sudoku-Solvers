//========================================================================================
// Name        : Pos.h
// Author      : Hongbo Tian (Created 14 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
//========================================================================================

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
