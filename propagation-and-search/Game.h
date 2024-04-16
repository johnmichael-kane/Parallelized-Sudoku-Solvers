// Name        : Game.h
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <functional>
#include <atomic>
#include <future>
#include "PossibleGrid.h"
#include "Grid.h"
#include "Position.h"

class Game {
private:
	bool hasInput = false;
	std::string puzzle;

	Grid gameGrid;
	PossibleGrid possibleGrid; 
	bool hasSolution = true;

	bool depthFirstSearch();
	bool parallelDepthFirstSearch();

public : 
	Game(std::string path);
	bool evaluateBoard();
	void printResult() const;

	int calculateFirstMovePossibilities()
	{
		possibleGrid.setGrid(&gameGrid);	 // Initialize the grid analysis
		possibleGrid.analyzeMoves(gameGrid); // Calculate possible moves based on current grid state

		// Retrieve unsolved positions from PossibleGrid
		const auto &unsolvedPositions = possibleGrid.getUnsolvedPositions();
		if (unsolvedPositions.empty())
		{
			return 0; // If no cells are unsolved, return 0 possibilities
		}

		// Find the position with the minimum remaining values using a lambda to compare the size of possible values
		auto minPosIt = std::min_element(unsolvedPositions.begin(), unsolvedPositions.end(),
										 [this](const Position &a, const Position &b)
										 {
											 return possibleGrid.getPossibleValuesAt(a.row, a.col).size() <
													possibleGrid.getPossibleValuesAt(b.row, b.col).size();
										 });

		// Return the number of possibilities for the most constrained cell
		return possibleGrid.getPossibleValuesAt(minPosIt->row, minPosIt->col).size();
	}
};

#endif /* GAME_H_ */