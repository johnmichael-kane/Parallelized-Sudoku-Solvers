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
	// Using a vector of vectors to dynamically store possible values for each cell
	std::vector<std::vector<std::vector<int>>> mPossibleGrid;
	std::vector<Pos> mUnsolvedPos;

private:
	// Filters the given vector to contain only unique, non-repeating elements.
	std::vector<int> nonRepeat(std::vector<int> vec);

	// Clears the possible values grid, preparing it for a new analysis.
	void clear();

public:
	// Constructor: Initializes a new instance of the PossibleGrid class.
	PossibleGrid();

	// Analyzes the given grid to determine possible values for each unsolved cell.
	// Ensure this takes a constant reference to a Grid object
	void Analysis(Grid &grid);

	// Performs cross-referencing to find cells with a unique possible value
	// by examining their row, column, and section.
	std::vector<std::pair<Pos, int>> crossRef();

	// Prints the possible values for each unsolved cell in the grid.
	void print();

	// Converts a vector of integers into a string representation.
	std::string vecToString(std::vector<int> &vec); // Ensure consistency with std:: prefix
};

#endif /* POSSIBLEGRID_H_ */
