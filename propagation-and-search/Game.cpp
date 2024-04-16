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

bool Game::evaluateBoard() {
	if (!hasInput || gameGrid.isComplete()) return gameGrid.isComplete(); // completion state

	possibleGrid.setGrid(&gameGrid); // initialize grid
	possibleGrid.analyzeMoves(gameGrid); // get possible values & unsolved positions
	bool cellsLeft = true;

	// (1) Linear Constraints: solve single-value positions.
	// TRY (LATER ON): PARALLELIZING THIS FOR GRID SIZES > 9
	int numUnsolvedCells = possibleGrid.getUnsolvedPositions().size();
	for (int i = 0; i < numUnsolvedCells; ++i) {
		const auto &pos = possibleGrid.getUnsolvedPositions()[i];
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1) {
			cellsLeft = false;
			gameGrid.fill(pos, possibleValues.front());
		}
	}

	// (2) Cross Reference: solve intersecting positions.
	if (cellsLeft) {
		// cout << "Cross Reference . . . " << endl;
		auto pairs = possibleGrid.crossReference();
		for (const auto &pair : pairs) {
			cellsLeft = false;
			gameGrid.fill(pair.first, pair.second);
		}
	}

	// (3) Search: solve leftover positions across grid.
	if (cellsLeft && !gameGrid.isComplete()) {
		hasSolution = depthFirstSearch();
	}

	return gameGrid.isComplete();
}

// TRY (SOON): PARALLELIZE THIS (only applies to larger grids anyways!!)
// problem: running too long - kept stopping on the puzzles DFS implemented in.
// instead: optimized DFS - MRV heuristic to choose next cell to fill
// Use parallel execution for the initial branching to quickly explore separate paths.
// Switch to a sequential DFS beyond a certain depth or when the workload for a thread falls below a certain threshold.
bool Game::depthFirstSearch()
{
	vector<Grid> searchQueue{gameGrid};

	while (!searchQueue.empty())
	{
		Grid currentGrid = std::move(searchQueue.back());
		searchQueue.pop_back(); // process (remove)

		if (currentGrid.isComplete())
		{
			gameGrid = std::move(currentGrid); // Found solution
			return true;
		}

		if (!currentGrid.isLegal())
			continue; // illegal -> skip

		PossibleGrid possibilities;
		possibilities.setGrid(&currentGrid);
		possibilities.analyzeMoves(currentGrid);

		auto unsolved = possibilities.getUnsolvedPositions();
		if (unsolved.empty())
			continue;

		auto pos = *std::min_element(unsolved.begin(), unsolved.end(), [&](const Position &a, const Position &b)
									 { return possibilities.getPossibleValuesAt(a.row, a.col).size() < possibilities.getPossibleValuesAt(b.row, b.col).size(); });

		auto posValues = possibilities.getPossibleValuesAt(pos.row, pos.col);
		for (int value : posValues)
		{
			Grid newGrid = currentGrid;
			newGrid.fill(pos, value);
			searchQueue.push_back(std::move(newGrid));
		}
	}

	return false;
}

void Game::printResult() const
{
	if (hasInput) {
		std::cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << std::endl;
		gameGrid.print();
	}
}