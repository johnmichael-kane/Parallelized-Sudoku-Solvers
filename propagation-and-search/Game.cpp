//========================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
// Description : Manage the game state, evaluate game progress, and print game statistics.
//========================================================================================

#include "Game.h"

Game::Game(const string &path) : gameFile(path), isFinished(false), hasSolution(true) {
	// Ensure grid read properly.
	if (!gameGrid.read(path))
	{
		cout << "Failed to read the grid from file: " << path << endl;
		hasSolution = false;
		return;
	}
	possibleGrid.setGrid(&gameGrid);
	possibleGrid.Analysis();
}

// Evaluate the current game state and try to make progress towards solution.
// 1) Update reference grid and re-analyze the current grid state.
// 2) First, attempt to solve the grid using linear constraints (simple eliminations).
// 3) If unsolved, then attempt to solve the grid using cross-referencing.
// 4) If still unsolved, then solve the grid using DFS.
// 5) Complete evaluation: determine if solution found.
bool Game::Evaluate() {
	if (!hasSolution) {
		cout << "No solution available due to initial read or logic failure." << endl;
		return true;
	}

	// Repeat solving attempts until no more progress can be made
	bool progress = true;
	while (progress && !gameGrid.isComplete())
	{
		progress = false;

		if (gameGrid.isLegal())
		{
			cout << "Grid is legal." << endl;
		}
		else
		{
			cout << "Grid is illegal." << endl;
			return false; // Early exit if grid is illegal
		}

		if (linearConstraintSolution())
		{
			std::cout << "Progress made using linear constraints." << std::endl;
			progress = true;
		}
		if (crossReferenceSolution())
		{
			std::cout << "Progress made using cross-referencing." << std::endl;
			progress = true;
		}
		if (!progress && !depthFirstSearch())
		{
			std::cout << "No progress can be made, DFS attempted but failed." << std::endl;
			hasSolution = false;
			return false;
		}
	}

	// Check if the grid is complete as the last step
	isFinished = gameGrid.isComplete();
	return isFinished;
}

// Prints game statistics and the grid if a solution is available.
void Game::printStats() const
{
	cout << (isFinished ? "Solved!!!" : "Has no Solution.") << endl;
	gameGrid.print();
}

// Attempt to fill single-option cells and return true if any cell was filled.
bool Game::linearConstraintSolution()
{
	bool progress = false;
	auto unsolvedPositions = gameGrid.getUnsolvedPositions();

	for (const auto &pos : unsolvedPositions) {
		const auto &possibilities = possibleGrid.possibleValues[pos.row][pos.col];
		if (possibilities.size() == 1) {
			gameGrid.fill(pos, possibilities.front());
			progress = true;
		}
	}

	return progress;
}

// Use cross-referencing to fill cells that can uniquely be determined.
bool Game::crossReferenceSolution() {
	auto uniquePositions = possibleGrid.crossReference();
	if (uniquePositions.empty())
	{
		return false;
	}

	for (const auto &item : uniquePositions)
	{
		gameGrid.fill(item.first, item.second);
	}

	return !uniquePositions.empty();
}

// Implements a depth-first search algorithm to explore possible grid configurations.
// Start queue of possible grids with initial game state.
// Create possible grid instance and analyze to update possible values.
// Update main game grid if solution found (grid complete).
// Otherwise, loop through each possible value from the first unsolved position.
bool Game::depthFirstSearch()
{
	std::vector<Grid> searchStack = {gameGrid};
	while (!searchStack.empty())
	{
		Grid currentGrid = searchStack.back();
		searchStack.pop_back(); // Process and remove the current grid

		if (!currentGrid.isLegal())
			continue; // Skip illegal grids

		PossibleGrid possibilities;
		possibilities.setGrid(&currentGrid); // Set the current grid for analysis
		possibilities.Analysis();			 // Analyze to find possible values for each cell

		auto unsolvedPositions = currentGrid.getUnsolvedPositions();
		if (unsolvedPositions.empty())
		{
			if (currentGrid.isComplete() && currentGrid.isLegal())
			{
				gameGrid = currentGrid; // If solution is found and is legal, copy it to main grid
				return true;
			}
			continue;
		}

		// Process the first unsolved position with multiple possibilities
		const auto &pos = unsolvedPositions[0];
		const auto &positionPossibilities = possibilities.possibleValues[pos.row][pos.col];
		for (int value : positionPossibilities)
		{
			Grid newGrid = currentGrid;
			newGrid.fill(pos, value);
			if (newGrid.isLegal())
			{ // Only add legal states to the stack
				searchStack.push_back(newGrid);
			}
		}
	}
	return false; // Return false if no solution is found
}
