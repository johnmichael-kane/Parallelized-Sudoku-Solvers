// Name        : PossibleGrid.h
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef POSSIBLEGRID_H_ 
#define POSSIBLEGRID_H_

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <utility> 
#include <iterator>
#include <numeric> 
#include "Grid.h" 
#include "Position.h"

class PossibleGrid {
private:
	Grid *grid;
	int gridSize;
	int sectionSize;
	std::vector<std::vector<std::vector<int>>> possibleValues;
	std::vector<Position> unsolvedPositions;
	std::vector<int> uniqueValues(std::vector<int> vec);
	std::vector<std::pair<int, Position>> workerFunction(const std::vector<std::vector<std::vector<int>>> &possibleValues,
														 int index, bool isRow, int start, int end) const;
	std::vector<std::pair<Position, int>> identifyUniqueValues(int index, bool isRow) const;

public:
	// Initialization
	PossibleGrid() : grid(nullptr), gridSize(0), sectionSize(0) {}

	void setGrid(Grid *g) {
		grid = g;
		if (grid) {
			gridSize = grid->getGridSize();
			sectionSize = grid->getSectionSize();
			clear();
	
			possibleValues.resize(gridSize, std::vector<std::vector<int>>(gridSize));
			unsolvedPositions = grid->getUnsolvedPositions();
		}
	}

	void clear() {
		possibleValues.clear();
		unsolvedPositions.clear();
	}

	// Analysis
	void analyzeMoves(const Grid &grid);
	std::vector<std::pair<Position, int>> crossReference() const;

	// Getters
	static std::vector<int> getUsedValues(const Grid &grid, const Position &position);
	const std::vector<Position> &getUnsolvedPositions() const { return unsolvedPositions; }
	const std::vector<int> &getPossibleValuesAt(int row, int col) const {
		return possibleValues[row][col];
	}
};

#endif /* POSSIBLEGRID_H_ */
