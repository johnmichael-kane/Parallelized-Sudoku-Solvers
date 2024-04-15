//==================================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Manage possible values for Sudoku grid cells to assist in solving Sudoku puzzles.
//==================================================================================================

#include "PossibleGrid.h"

void PossibleGrid::analyzeMoves(const Grid &grid) {
	if (this->grid == nullptr) return;

	clear();
	possibleValues.resize(gridSize, vector<vector<int>>(gridSize));
	unsolvedPositions = grid.getUnsolvedPositions();

	// fill vector with [1 ... gridSize]
	const vector<int> range = [&] {
		vector<int> r(gridSize);
		iota(r.begin(), r.end(), 1); 
		return r;
	}();

	vector<int> usedValues(gridSize);

	for (const auto &pos : unsolvedPositions) {
		const vector<int> &usedSectionValues = grid.getSection(pos.row, pos.col);
		const vector<int> &usedRowValues = grid.getRow(pos.row);
		const vector<int> &usedColValues = grid.getCol(pos.col);

		usedValues.clear();
		usedValues.insert(usedValues.end(), usedSectionValues.begin(), usedSectionValues.end());
		usedValues.insert(usedValues.end(), usedRowValues.begin(), usedRowValues.end());
		usedValues.insert(usedValues.end(), usedColValues.begin(), usedColValues.end());

		sort(usedValues.begin(), usedValues.end());
		usedValues.erase(unique(usedValues.begin(), usedValues.end()), usedValues.end()); // no dups

		vector<int> possibilities;
		set_difference(range.begin(), range.end(), usedValues.begin(), usedValues.end(),
					   back_inserter(possibilities));
		possibleValues[pos.row][pos.col] = std::move(possibilities);
	}
}

// Cross-reference rows and columns and return unique possible value pairs (position & value).
vector<pair<Position, int>> PossibleGrid::crossReference() const {
	vector<pair<Position, int>> pairs;
	for (int i = 0; i < gridSize; ++i) {
		auto rowUnique = identifyUnique(i, true);  // ith row possibilities
		auto colUnique = identifyUnique(i, false); // ith col possibilities
		pairs.insert(pairs.end(), rowUnique.begin(), rowUnique.end());
		pairs.insert(pairs.end(), colUnique.begin(), colUnique.end());
	}
	return pairs;
}

// Identify unique values in a specified row/column
vector<pair<Position, int>> PossibleGrid::identifyUnique(int index, bool isRow) const {
	multimap<int, Position> valueMap; // map: quick lookup
	for (int i = 0; i < gridSize; ++i) {
		const vector<int> &values = isRow ? possibleValues[index][i] : possibleValues[i][index];
		for (int val : values) {
			valueMap.emplace(val, isRow ? Position(index, i) : Position(i, index));
		}
	}

	vector<pair<Position, int>> uniquePairs;
	for (auto it = valueMap.begin(); it != valueMap.end(); ++it) { // auto = type-safe
		const auto &value = it->first;  // key
		const auto &position = it->second; // value 

		if (valueMap.count(value) == 1) { // appears once (unique)
			uniquePairs.push_back(make_pair(position, value));
		}
	}

	return uniquePairs;
}
