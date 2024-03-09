#ifndef POSSIBLEGRID_H_ // Corrected typo in the include guard
#define POSSIBLEGRID_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "Grid.h" // Includes the definition of the Grid class
#include "Pos.h"  // Includes the definition of the Pos class

// **** SIZE: ALTER TO ADJUST TO IT PROPERLY ****

// The PossibleGrid class is responsible for analyzing a Sudoku grid and determining
// the possible values for each unsolved cell.
class PossibleGrid
{
public:
	vector<int> mPossibleGrid[Grid::MAX][Grid::MAX]; // 2D array storing possible values for each cell
	vector<Pos> mUnsolvedPos;						 // List of positions in the grid that are not yet solved

private:
	// Filters the given vector to contain only unique, non-repeating elements.
	vector<int> nonRepeat(vector<int> vec);

	// Clears the possible values grid, preparing it for a new analysis.
	void clear();

public:
	// Constructor: Initializes a new instance of the PossibleGrid class.
	PossibleGrid();

	// Analyzes the given grid to determine possible values for each unsolved cell.
	void Analysis(Grid grid);

	// Performs cross-referencing to find cells with a unique possible value
	// by examining their row, column, and section.
	vector<pair<Pos, int>> crossRef();

	// Prints the possible values for each unsolved cell in the grid.
	void print();

	// Converts a vector of integers into a string representation. **** WHY? ****
	string vecToString(vector<int> vec);
};

#endif /* POSSIBLEGRID_H_ */
