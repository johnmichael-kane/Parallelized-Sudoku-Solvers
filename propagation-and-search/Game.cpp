//========================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : ?
//========================================================================================

#include "Game.h"

using namespace std;

Game::Game(string path) : puzzle(path) { hasInput = gameGrid.read(path); }

bool Game::evaluateBoard() {
	if (!hasInput || gameGrid.isComplete()) return gameGrid.isComplete(); // completion state

	possibleGrid.setGrid(&gameGrid); // initialize grid
	possibleGrid.analyzeMoves(gameGrid); // get possible values & unsolved positions
	bool playGame = true;

	// (1) Linear Constraints: solve single-value positions.
	// TRY (LATER ON): PARALLELIZING THIS FOR GRID SIZES > 9
	int numUnsolvedCells = possibleGrid.getUnsolvedPositions().size();
	for (int i = 0; i < numUnsolvedCells; ++i) {
		const auto &pos = possibleGrid.getUnsolvedPositions()[i];
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1) {
			playGame = false;
			gameGrid.fill(pos, possibleValues.front());
		}
	}

	// (2) Cross Reference: solve intersecting positions.
	if (playGame) {
		cout << "Cross Reference . . . " << endl;
		auto pairs = possibleGrid.crossReference();
		for (const auto &pair : pairs) {
			playGame = false;
			gameGrid.fill(pair.first, pair.second);
		}
	}

	// (3) Search: solve leftover positions across grid.
	if (playGame && !gameGrid.isComplete()) {
		hasSolution = depthFirstSearch();
	}

	return gameGrid.isComplete();
}

bool Game::depthFirstSearch() {
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

	return false;
}

void Game::printResult() const
{
	if (hasInput) {
		std::cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << std::endl;
		gameGrid.print();
	}
}