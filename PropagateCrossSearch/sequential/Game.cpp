//==================================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Implements methods to solve a Sudoku board using linear constraint propagation,
//               cross-referencing, and depth-first search (DFS), potentially in parallel based on
//               puzzle difficulty. Returns true if the board is solved.
//==================================================================================================

#include "Game.h"
#include <vector>
#include <iostream>
using namespace std;

Game::Game(string path) : puzzlePath(path)
{
	hasInput = gameGrid.read(path);
	possibleGrid.setGrid(&gameGrid);		   // Initialize grid with possible values
}  

bool Game::evaluateBoard()
{
	if (!hasInput || gameGrid.isComplete())
		return gameGrid.isComplete();

	possibleGrid.analyzeMoves(gameGrid); // Calculate possible moves
	bool cellsLeft = true;

	auto unsolvedCells = possibleGrid.getUnsolvedPositions();
	for (const auto &pos : unsolvedCells)
	{
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1)
		{
			gameGrid.fill(pos, possibleValues.front()); // Fill in the single possible value
			cellsLeft = false;
		}
	}

	if (cellsLeft)
	{
		auto pairs = possibleGrid.crossReference();
		for (const auto &pair : pairs)
		{
			cellsLeft = false;
			gameGrid.fill(pair.first, pair.second); // Fill intersecting cells
		}
	}

	if (cellsLeft && !gameGrid.isComplete())
	{
		hasSolution = depthFirstSearch();
	}

	return gameGrid.isComplete(); // Return true if the board is completely solved
}

int Game::calculateFirstMovePossibilities()
{
	auto unsolvedPositions = possibleGrid.getUnsolvedPositions();
	if (unsolvedPositions.empty())
		return 0;

	auto minPosIt = min_element(unsolvedPositions.begin(), unsolvedPositions.end(),
								[this](const Position &a, const Position &b)
								{
									return possibleGrid.getPossibleValuesAt(a.row, a.col).size() <
										   possibleGrid.getPossibleValuesAt(b.row, b.col).size();
								});

	return possibleGrid.getPossibleValuesAt(minPosIt->row, minPosIt->col).size();
}

bool Game::depthFirstSearch()
{
	vector<Grid> searchStack = {gameGrid}; // Use a vector as a stack for DFS

	while (!searchStack.empty())
	{
		Grid currentGrid = std::move(searchStack.back());
		searchStack.pop_back();

		if (currentGrid.isComplete())
		{
			gameGrid = std::move(currentGrid);
			return true; // Puzzle solved
		}

		if (!currentGrid.isLegal())
			continue; // Skip illegal grid configurations

		PossibleGrid possibilities;
		possibilities.setGrid(&currentGrid);
		possibilities.analyzeMoves(currentGrid);

		auto unsolvedPositions = possibilities.getUnsolvedPositions();
		if (unsolvedPositions.empty())
			continue;

		auto pos = *min_element(unsolvedPositions.begin(), unsolvedPositions.end(),
								[&possibilities](const Position &a, const Position &b)
								{
									return possibilities.getPossibleValuesAt(a.row, a.col).size() <
										   possibilities.getPossibleValuesAt(b.row, b.col).size();
								});

		for (int value : possibilities.getPossibleValuesAt(pos.row, pos.col))
		{
			Grid newGrid = currentGrid;
			newGrid.fill(pos, value);
			searchStack.push_back(std::move(newGrid)); // Push new state to stack
		}
	}

	return false; // No solution found
}

void Game::printResult() const
{
	if (hasInput)
	{
		cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << endl;
		gameGrid.print();
	}
}