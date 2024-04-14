// Name        : PossibleGrid.h
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef POSSIBLEGRID_H_ 
#define POSSIBLEGRID_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // std::sort, std:: move, etc.
#include <utility> // std::move
#include <iterator> // std::distance
#include <sstream> // std::stringstream
#include "Grid.h" 
#include "Position.h" 

class PossibleGrid {
public:
	PossibleGrid();
	void setGrid(Grid *g) { this->grid = g; }
	void print() const;

	// Analysis
	void analyzeMoves(const Grid &grid);
	vector<pair<Position, int>> crossReference() const;

	// Getters
	const std::vector<Position> &getUnsolvedPositions() const { return unsolvedPositions; }
	const std::vector<int> &getPossibleValuesAt(int row, int col) const {
		if (row < 0 || row >= possibleValues.size() || col < 0 || col >= possibleValues[row].size()) {
			throw std::out_of_range("Accessing possibleValues out of range");
		}
		return possibleValues[row][col];
	}

private:
	Grid *grid;
	vector<vector<vector<int>>> possibleValues;
	vector<Position> unsolvedPositions;

	void clear();
	string vectorToString(const std::vector<int> &vec) const;

	// Uniqueness
	vector<int> uniqueValues(vector<int> vec);
	std::vector<std::pair<Position, int>> identifyUnique(int index, bool isRow) const;
	std::vector<std::pair<Position, int>> filterUnique(const std::vector<int> &collection, 
													   const std::multimap<int, 
													   Position> &valueMap) const;
};

#endif /* POSSIBLEGRID_H_ */
