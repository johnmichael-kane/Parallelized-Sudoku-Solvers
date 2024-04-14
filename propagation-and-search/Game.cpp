//========================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
// Description : Manage the game state, evaluate game progress, and print game statistics.
//========================================================================================

#include "Game.h"

Game::Game(string path) : gameFile(path),	 
						  isFinished(false),
						  hasSolution(true),
						  linearCycle(0),	 // Counter for linear constraint cycles.
										  // apply rules to individual rows/columns/3x3 subgrids **** SIZE ****
										  // rule: each row/column/subgrid mustcontain 1-9 exactly once
										  // deduce by eliminating all other vals present
										  // solve the simpler parts straightforwardly
						  crossRefCycle(0), // Counter for cross-reference cycles.
											// cross-reference different rows/columns/subgrids
											// look at cell vals & see if potential vals are impossible in other
											// parts of the same row/col/subgrid due to placement of numbers
											// in related rows/cols/subgrids
											// eliminate possible vals for cell that can't exist due to linear structures
											// make progress on puzzles that need more than linear constraints to solve
						  searchCycle(0) // Counter for search cycles
										 // pick an unsolved cell, assume possible val, continue solving based on val
										 // if leads to contradiction (unsolvable) backtrack & mark val incorrect
										 // DFS
										 // process repeats until puzzle solution found or deemed unsolvable
										 // intensive;last resort when other methods insufficient
{
	possibleGameGrid.setGrid(&gameGrid);
	possibleGameGrid.Analysis(gameGrid);
}

// Evaluate the current game state and try to make progress towards solution.
// 1) Update reference grid and re-analyze the current grid state.
// 2) First, attempt to solve the grid using linear constraints (simple eliminations).
// 3) If unsolved, then attempt to solve the grid using cross-referencing.
// 4) If still unsolved, then solve the grid using DFS.
// 5) Complete evaluation: determine if solution found.
bool Game::Evaluate()
{
	possibleGameGrid.setGrid(&gameGrid); 
	possibleGameGrid.Analysis(gameGrid);

	if (!linearConstraintSolution())
	{
		if (!crossReferenceSolution())
		{
			if (!gameGrid.isComplete())
			{
				isFinished = search();
			}
		}
	}
	return gameGrid.isComplete(); 
}

// Attempt to fill single-option cells and return true if any cell was filled.
bool Game::linearConstraintSolution()
{
	bool progressMade = false;
	for (Pos &pos : possibleGameGrid.unsolvedPositions)
	{
		if (possibleGameGrid.possibleGrid[pos.row][pos.col].size() == 1)
		{
			gameGrid.fill(pos, possibleGameGrid.possibleGrid[pos.row][pos.col][0]);
			progressMade = true;
		}
	}
	return progressMade;
}

// Use cross-referencing to fill cells that can uniquely be determined.
bool Game::crossReferenceSolution()
{
	vector<pair<Pos, int>> pairs = possibleGameGrid.crossReference();
	if (pairs.empty())
		return false;
	for (auto &[pos, value] : pairs)
	{
		gameGrid.fill(pos, value);
	}
	return true;
}

// Implements a depth-first search algorithm to explore possible grid configurations.
// Start queue of possible grids with initial game state.
// Create possible grid instance and analyze to update possible values.
// Update main game grid if solution found (grid complete).
// Otherwise, loop through each possible value from the first unsolved position.
bool Game::search()
{
	cout << "Depth First Search . . . " << endl;
	vector<Grid> searchQueue = {gameGrid};

	while (!searchQueue.empty())
	{
		Grid currentGrid = searchQueue.back();
		searchQueue.pop_back(); // process grid (remove)

		if (!currentGrid.isLegal()) continue; // illegal grid (skip)

		PossibleGrid possibilities; 
		possibilities.Analysis(currentGrid);

		if (currentGrid.isComplete())
		{
			gameGrid = currentGrid;
			return true;
		}

		Pos pos = possibilities.getFirstUnsolvedPosition();
		if (pos.row == -1 && pos.col == -1) continue; // no unsolved (skip)

		for (int value : possibilities.possibleGrid[pos.row][pos.col])
		{
			Grid tempGrid = currentGrid;
			tempGrid.fill(pos, value);
			searchQueue.emplace_back(tempGrid);
		}
	}

	hasSolution = false;
	return false;
}

// Prints game statistics and the grid if a solution is available.
void Game::printStats()
{
	cout << (hasSolution ? "Solved!!!" : "Has no Solution.") << endl;
	gameGrid.print();
}