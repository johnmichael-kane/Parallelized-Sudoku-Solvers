//==================================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Implement (1) linear constraint propagation, (2) cross-referencing (if necessary),
// 				 and (3) a DFS (if necessary) to solve a Sudoku board. Return true if solved.
//==================================================================================================

#include "Game.h"

using namespace std;

Game::Game(string path) : puzzle(path)
{
	hasInput = gameGrid.read(path);
}

bool Game::evaluateBoard()
{
	if (!hasInput || gameGrid.isComplete())
		return gameGrid.isComplete(); // completion state

	possibleGrid.setGrid(&gameGrid);	 // initialize grid
	possibleGrid.analyzeMoves(gameGrid); // get possible values & unsolved positions
	bool cellsLeft = true;

	// (1) Linear Constraints: solve single-value positions.
	// TRY (LATER ON): PARALLELIZING THIS FOR GRID SIZES > 9
	int numUnsolvedCells = possibleGrid.getUnsolvedPositions().size();
	for (int i = 0; i < numUnsolvedCells; ++i)
	{
		const auto &pos = possibleGrid.getUnsolvedPositions()[i];
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1)
		{
			cellsLeft = false;
			gameGrid.fill(pos, possibleValues.front());
		}
	}

	// (2) Cross Reference: solve intersecting positions.
	if (cellsLeft)
	{
		// cout << "Cross Reference . . . " << endl;
		auto pairs = possibleGrid.crossReference();
		for (const auto &pair : pairs)
		{
			cellsLeft = false;
			gameGrid.fill(pair.first, pair.second);
		}
	}

	// (3) Search: solve leftover positions across grid.
	if (cellsLeft && !gameGrid.isComplete())
	{
		if (numUnsolvedCells > 9)
		{ // Example condition: more complexity requires parallel processing
			hasSolution = parallelDepthFirstSearch();
		}
		else
		{
			hasSolution = depthFirstSearch();
		}
	}

	return gameGrid.isComplete();
}

// Here's a simplified example of how you might refactor the handling of futures in parallelDepthFirstSearch for better efficiency:
bool Game::parallelDepthFirstSearch()
{
	// Determine the number of possibilities for the most constrained cell
	int first_move_possibilities = calculateFirstMovePossibilities();

	// Calculate the initial branches based on the system's hardware concurrency,
	// but limit it by the first move possibilities to avoid unnecessary parallelism.
	int initial_branches = std::min(static_cast<unsigned int>(std::thread::hardware_concurrency()),
									static_cast<unsigned int>(first_move_possibilities));

	// Ensure there's at least one branch if the grid isn't trivially simple
	initial_branches = std::max(1, initial_branches);

	vector<future<bool>> futures;

	// Launch each branch to run depthFirstSearch asynchronously
	for (int i = 0; i < initial_branches; ++i)
	{
		futures.push_back(async(launch::async, [this]
								{ return this->depthFirstSearch(); }));
	}

	// Wait for any branch to find a solution
	for (auto &fut : futures)
	{
		if (fut.get()) // If any future returns true, a solution has been found
		{
			return true; // Stop all and return true
		}
	}

	return false;
}

// TRY (SOON): PARALLELIZE THIS (only applies to larger grids anyways!!)
// problem: running too long - kept stopping on the puzzles DFS implemented in.
// instead: optimized DFS - MRV heuristic to choose next cell to fill
// Use parallel execution for the initial branching to quickly explore separate paths.
// Switch to a sequential DFS beyond a certain depth or when the workload for a thread falls below a certain threshold.
// hybrid implementation fixed my code!!
bool Game::depthFirstSearch()
{
	std::vector<Grid> searchStack;	 // Using a vector as a stack for DFS
	searchStack.push_back(gameGrid); // Start with the initial game grid

	while (!searchStack.empty())
	{
		Grid currentGrid = std::move(searchStack.back());
		searchStack.pop_back(); // Pop the last element to continue the DFS

		if (currentGrid.isComplete())
		{
			gameGrid = std::move(currentGrid); // If complete, we found a solution
			return true;
		}

		if (!currentGrid.isLegal())
		{
			continue; // Skip this grid if it's not a legal configuration
		}

		// Set up to analyze possible moves based on current grid state
		PossibleGrid possibilities;
		possibilities.setGrid(&currentGrid);
		possibilities.analyzeMoves(currentGrid);

		// Use a heuristic to choose the next cell to try filling
		auto unsolvedPositions = possibilities.getUnsolvedPositions();
		if (unsolvedPositions.empty())
		{
			continue; // No unsolved positions left, but grid is not complete
		}

		// Select the position with the minimum remaining values for filling
		auto pos = *std::min_element(unsolvedPositions.begin(), unsolvedPositions.end(),
									 [&possibilities](const Position &a, const Position &b)
									 {
										 return possibilities.getPossibleValuesAt(a.row, a.col).size() < possibilities.getPossibleValuesAt(b.row, b.col).size();
									 });

		auto possibleValues = possibilities.getPossibleValuesAt(pos.row, pos.col);
		for (int value : possibleValues)
		{
			Grid newGrid = currentGrid;
			newGrid.fill(pos, value);				   // Try filling the chosen cell with a possible value
			searchStack.push_back(std::move(newGrid)); // Push the new grid state onto the stack
		}
	}

	return false; // No solution found after exploring all possibilities
}

void Game::printResult() const
{
	if (hasInput)
	{
		std::cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << std::endl;
		gameGrid.print();
	}
}