//========================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : ?
//========================================================================================

#include "Game.h"

Game::Game(string path) : puzzle(path) {
	hasSuccessInput = gameGrid.read(path);
	if (hasSuccessInput) {
		possibleGrid.setGrid(&gameGrid);
		possibleGrid.analyzeMoves(gameGrid);
	}
}

bool Game::Evaluate() {
	if (!hasSuccessInput) return false;

	possibleGrid.setGrid(&gameGrid);
	possibleGrid.analyzeMoves(gameGrid);
	bool trigger = true;

	// Linear Constraints: solve single-value positions.
	for (const auto &pos : possibleGrid.getUnsolvedPositions()) {
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1) {
			trigger = false;
			gameGrid.fill(pos, possibleValues.front());
		}
	}

	// Cross Reference: solve intersecting positions.
	if (trigger) {
		auto pairs = possibleGrid.crossReference();
		for (const auto &pair : pairs) {
			trigger = false;
			gameGrid.fill(pair.first, pair.second);
		}
	}

	// Search: solve leftover positions across grid.
	if (trigger && !gameGrid.isComplete()) {
		isFinished = search();
	}

	return gameGrid.isComplete();
}

bool Game::search()
{
	cout << "Depth First Search . . . " << endl;
	bool success = true;

	vector<Grid> searchQueue;
	searchQueue.push_back(gameGrid);
	while (!searchQueue.operator[](searchQueue.size() - 1).isComplete() && searchQueue.size() > 0)
	{
		searchCycle++; 
		
		Grid currentGrid = searchQueue.at(searchQueue.size() - 1);

		if (currentGrid.isLegal())
		{
			PossibleGrid possGrid;
			possGrid.analyzeMoves(currentGrid);
			searchQueue.pop_back();
			Position pos = possGrid.getUnsolvedPositions().at(0);
			vector<int> posValues = possGrid.getPossibleValuesAt(pos.row, pos.col);

			for (vector<int>::iterator it = posValues.begin(); it != posValues.end(); it++)
			{
				Grid tempGrid = currentGrid;
				tempGrid.fill(pos, *it);
				searchQueue.push_back(tempGrid);
			}
		}
		else
		{
			searchQueue.pop_back();
		}
	}

	if (!searchQueue.empty()) 
	{
		success = true;
		gameGrid = searchQueue.back();
	}
	else
	{
		hasSolution = false;
	}
	return success; 
}

void Game::printResult() const
{
	stringstream ss;

	if (hasSolution)
	{
		ss << "Solved!!!";
	}
	else
	{
		ss << "Has no Solution.";
	}

	if (hasSuccessInput)
	{
		cout << ss.str() << endl;
		gameGrid.print();
	}
}