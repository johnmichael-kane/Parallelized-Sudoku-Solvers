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

bool Game::search() {
	cout << "Depth First Search . . . " << endl;
	vector<Grid> searchQueue{gameGrid};

	while (!searchQueue.empty()) {
		Grid currentGrid = std::move(searchQueue.back());
		searchQueue.pop_back(); // process (remove)

		if (!currentGrid.isComplete()) {
			if (!currentGrid.isLegal()) continue; // illegal -> skip

			PossibleGrid possibilities;
			possibilities.setGrid(&currentGrid);
			possibilities.analyzeMoves(currentGrid);
			const auto &pos = possibilities.getUnsolvedPositions().front();
			const auto &posValues = possibilities.getPossibleValuesAt(pos.row, pos.col);

			for (int value : posValues) { // position
				Grid newGrid = currentGrid;
				newGrid.fill(pos, value);
				searchQueue.push_back(std::move(newGrid)); // "move on"
			}
		} else {
			gameGrid = std::move(currentGrid); // "move on"
			return true;
		}
	}

	hasSolution = false;
	return false;
}

void Game::printResult() const
{
	if (hasSuccessInput) {
		std::cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << std::endl;
		gameGrid.print();
	}
}