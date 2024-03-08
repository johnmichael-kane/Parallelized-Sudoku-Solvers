/*
 * Game.cpp
 *
 *  Created on: 15 Jul 2015
 *      Author: Hongbo Tian
 */

#include "Game.h"

// Constructor: Initializes a game with a specified path for the input file.
Game::Game(string path) : mPath(path),		  // Path to the input file.
						  mFin(false),		  // Flag to check if the game has finished.
						  mHasSolution(true), // Flag to indicate if the game has a solution.
						  mLinearCycle(0),	  // Counter for linear constraint cycles.
											  // apply rules to individual rows/columns/3x3 subgrids **** SIZE ****
											  // rule: each row/column/subgrid mustcontain 1-9 exactly once
											  // deduce by eliminating all other vals present
											  // solve the simpler parts straightforwardly
						  mCrossRefCycle(0),  // Counter for cross-reference cycles.
											  // cross-reference different rows/columns/subgrids
											  // look at cell vals & see if potential vals are impossible in other
											  // parts of the same row/col/subgrid due to placement of numbers
											  // in related rows/cols/subgrids
											  // eliminate possible vals for cell that can't exist due to linear structures
											  // make progress on puzzles that need more than linear constraints to solve
						  mSearchCycle(0)	  // Counter for search cycles
											  // pick an unsolved cell, assume possible val, continue solving based on val
											  // if leads to contradiction (unsolvable) backtrack & mark val incorrect
											  // DFS
											  // process repeats until puzzle solution found or deemed unsolvable
											  // intensive;last resort when other methods insufficient
{
	// Try to read the game grid from the file at the given path.
	// mSucessInput is true if the file is successfully read and false otherwise.
	mSucessInput = mGameGrid.read(path);
	// Analyze the initial game grid to populate possible values. **** SIZE ****
	mGamePossibleGrid.Analysis(mGameGrid);
}

// Evaluates the current state of the game and attempts to progress towards a solution.
bool Game::Evaluate()
{
	// Only proceed if the input file was successfully read.
	if (mSucessInput)
	{
		// Re-analyze the game grid to update possible values.
		mGamePossibleGrid.Analysis(mGameGrid);
		bool methodTrigger = true; // A flag to trigger different solving methods.

		// Linear Constrains Part 1: Solve positions with only one possible value.
		// Look for elements with only one possible solution
		vector<Pos>::iterator PosIt;
		for (PosIt = mGamePossibleGrid.mUnsolvedPos.begin();
			 PosIt != mGamePossibleGrid.mUnsolvedPos.end(); PosIt++)
		{
			// If a position has exactly one possible value, fill it in the grid.
			if (mGamePossibleGrid.mPossibleGrid[PosIt->row][PosIt->col].size() == 1)
			{
				methodTrigger = false; // Reset the flag as we've made progress.
				mGameGrid.fill(*PosIt,
							   mGamePossibleGrid.mPossibleGrid[PosIt->row][PosIt->col].at(0));
			}
		}

		// Linear Constrains Part 2: Cross-reference rows, columns, and sections to find solutions.
		// The core of this method is implemented in PossibleGrid
		// returns a array of pairs with Pos and value to fill
		if (methodTrigger)
		{
			// crossRef returns a list of positions and their determined values.
			vector<pair<Pos, int>> pairs = mGamePossibleGrid.crossRef();
			vector<pair<Pos, int>>::iterator it;
			for (it = pairs.begin(); it != pairs.end(); it++)
			{
				methodTrigger = false;				   // Again, reset the flag as progress is made.
				mGameGrid.fill(it->first, it->second); // Fill in the grid with the solved values.
			}
		}

		// Attempt a depth-first search if other methods fail to solve the grid completely.
		if (methodTrigger && !mGameGrid.isComplete())
		{
			mFin = search(); // The search method returns true if the grid is solved.
		}
		return mGameGrid.isComplete(); // Return true if the grid is complete, false otherwise.
	}
	else
	{
		// If the input file was not successfully read, indicate an error.
		cout << "Invalid File Name" << endl;
		return false; // Return false as the game cannot proceed without a valid input.
	}
}

// Implements a depth-first search algorithm to solve the grid.
bool Game::search()
{
	// Depth first search
	cout << "Depth First Search . . . " << endl;
	bool success = true;

	// Initialize the search queue with the current state of the game grid.
	vector<Grid> searchQueue;
	searchQueue.push_back(mGameGrid);
	// Continue searching until the last grid in the queue is complete or the queue is empty.
	while (!searchQueue.operator[](searchQueue.size() - 1).isComplete() && searchQueue.size() > 0)
	{
		mSearchCycle++; // Increment the search cycle counter.

		// Take the last grid from the queue to analyze.
		Grid currentGrid = searchQueue.at(searchQueue.size() - 1);

		// Test element in queue
		// If the current grid state is legal, try to find further solutions.
		if (currentGrid.isLegal())
		{
			PossibleGrid possGrid; // Temporary structure to analyze possible values. **** ERROR? ****
			possGrid.Analysis(currentGrid); // Analyze the current grid.
			searchQueue.pop_back(); // Remove the analyzed grid from the queue.
			Pos pos = possGrid.mUnsolvedPos.at(0); // Get the first unsolved position.
			vector<int> posValues = possGrid.mPossibleGrid[pos.row][pos.col]; // Get possible values for this position.
			// For each possible value, create a new grid with that value filled in and add it to the queue.
			for (vector<int>::iterator it = posValues.begin(); it != posValues.end(); it++)
			{
				Grid tempGrid = currentGrid;
				tempGrid.fill(pos, *it);
				searchQueue.push_back(tempGrid);
			}
		}
		else
		{
			// If the current grid is not legal, remove it from the queue.
			searchQueue.pop_back();
		}
	}

	if (searchQueue.size() > 0)
	{
		success = true;
		mGameGrid = searchQueue.at(searchQueue.size() - 1);
	}
	else
	{
		mHasSolution = false;
	}
	return success;
}

// Prints statistics and the solution if available.
void Game::printStats()
{
	stringstream ss;

	if (mHasSolution)
	{
		ss << "Solved!!!";
	}
	else
	{
		ss << "Has no Solution.";
	}
	if (mSucessInput)
	{
		cout << ss.str() << endl;
		mGameGrid.print();
	}
}
