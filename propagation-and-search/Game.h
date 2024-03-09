/*
 * Game.h
 *
 *  Created on: 15 Jul 2015
 *      Author: Hongbo Tian
 *  Updated on: 8 Mar 2024
 * 		Editor: Soleil Cordray
 */

// Game.h: Header file for the Game class, which encapsulates the logic and state for a puzzle game.
// This class is responsible for managing the game state, evaluating game progress, and printing game statistics.

#ifndef GAME_H_ // Preprocessor directive to ensure this header is only included once.
#define GAME_H_

#include <iostream>		  // Include for input/output streaming operations.
#include <vector>		  // Include for using the vector container.
#include "PossibleGrid.h" // Include the header for the PossibleGird class, presumably a typo and should be "PossibleGrid". **** FIX ****
#include "Grid.h"		  // Include the header for the Grid class, representing the game's grid.
#include "Pos.h"		  // Include the header for the Pos class, likely representing a position within the grid.

using namespace std; // Use the standard namespace to avoid prefixing STL components with 'std::'.

class Game
{
private:
	string mPath;					// Path to the game file.
	bool mFin;						// Flag to indicate if the game has finished.
	bool mHasSolution;				// Flag to indicate if the game has a solution.
	bool mSucessInput;				// Flag to indicate if the game input (file) was successfully loaded, typo should be "mSuccessInput".
	Grid mGameGrid;					// Instance of Grid to hold the current state of the game grid.
	PossibleGrid mGamePossibleGrid; // Instance of PossibleGrid to manage possible values for grid positions.
	int mLinearCycle;				// Counter for the number of linear constraint cycles performed.
	int mCrossRefCycle;				// Counter for the number of cross-reference cycles performed.
	int mSearchCycle;				// Counter for the number of search cycles (e.g., depth-first search) performed.
	bool search();					// Private method for the search strategy to solve the game.

public:
	// Constructor that initializes a Game object with a path to the input file.
	Game(string path);
	// Method to evaluate the current game state and attempt to make progress towards solving the game.
	bool Evaluate();
	// Method to print the game's current state and statistics about the solving process.
	void printStats();
};

#endif /* GAME_H_ */ // End of the preprocessor conditional directive.
