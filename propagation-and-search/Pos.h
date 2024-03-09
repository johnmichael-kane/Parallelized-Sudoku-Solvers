/*
 * Pos.h
 *
 *  Created on: 14 Jul 2015
 *      Author: eva
 *  Updated on: 8 Mar 2024
 * 		Editor: Soleil Cordray
 */

#ifndef POS_H_
#define POS_H_

#include <sstream> // Include for std::stringstream, used in toString() methodusing namespace std;
using namespace std; // Use the standard namespace to avoid prefixing standard library types with 'std::'.

// Define the Pos structure, which represents a position in a two-dimensional grid (like a Sudoku grid).
struct Pos {
	int row;
	int col;

	// Constructor for the Pos structure. Initializes a Pos instance with the specified row and column indices.
	Pos(int row, int col);
	string toString();

	/* potential replacement for toString:
	string toString() {
		stringstream ss; // Create a stringstream instance for assembling the string.
		ss << "(" << row << ", " << col << ")"; // Append the row and column indices to the stringstream.
		return ss.str(); // Convert the stringstream to a string and return it.
	} */
};

#endif /* POS_H_ */
