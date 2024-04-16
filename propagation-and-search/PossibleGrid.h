// Name        : PossibleGrid.h
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef POSSIBLEGRID_H_ 
#define POSSIBLEGRID_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // sort, move, etc.
#include <utility> // move
#include <iterator> // distance
#include <numeric> // iota
#include "Grid.h" 
#include "Position.h"

class PossibleGrid {
public:
	// Initialization
	PossibleGrid() : grid(nullptr), gridSize(0), sectionSize(0) {}

	void setGrid(Grid *g) {
		grid = g;
		if (grid) {
			gridSize = grid->getGridSize();
			sectionSize = grid->getSectionSize();
			clear();
		}
	}

	void clear() {
		possibleValues.clear();
		unsolvedPositions.clear();
		possibleValues.resize(gridSize, std::vector<std::vector<int>>(gridSize));
	}

	// Analysis
	void analyzeMoves(const Grid &grid);
	std::vector<std::pair<Position, int>> crossReference() const;

	// Getters
	const std::vector<Position> &getUnsolvedPositions() const { return unsolvedPositions; }
	const std::vector<int> &getPossibleValuesAt(int row, int col) const {
		return possibleValues[row][col];
	}
private:
	Grid *grid;
	int gridSize;
	int sectionSize;
	std::vector<std::vector<std::vector<int>>> possibleValues;
	std::vector<Position> unsolvedPositions;
	std::vector<int> uniqueValues(std::vector<int> vec);
	std::vector<std::pair<Position, int>> identifyUniqueValues(int index, bool isRow) const;
};

#endif /* POSSIBLEGRID_H_ */
