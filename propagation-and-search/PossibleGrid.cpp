//==================================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Manage possible values for Sudoku grid cells to assist in solving Sudoku puzzles.
//==================================================================================================

#include "PossibleGrid.h"

PossibleGrid::PossibleGrid() : grid(nullptr) {}

void PossibleGrid::clear() {
	possibleValues.clear();
	unsolvedPositions.clear();
}

void PossibleGrid::analyzeMoves(const Grid &grid) {
	if (this->grid == nullptr)
	{
		std::cerr << "Grid pointer not set.";
		return; // Exit if grid is not initialized
	}

	clear();
	possibleValues.resize(grid.gridSize, vector<vector<int>>(grid.gridSize));
	unsolvedPositions = grid.getUnsolvedPositions();
	vector<int> possibilities(grid.gridSize);
	const vector<int> range(1, grid.gridSize + 1);

	for (const auto &pos : unsolvedPositions) {
		const vector<int> usedSectionValues = grid.getSection(pos.row, pos.col);
		const vector<int> usedRowValues = grid.getRow(pos.row);
		const vector<int> usedColValues = grid.getCol(pos.col);

		vector<int> usedValues(usedSectionValues);
		usedValues.insert(usedValues.end(), usedRowValues.begin(), usedRowValues.end());
		usedValues.insert(usedValues.end(), usedColValues.begin(), usedColValues.end());

		sort(usedValues.begin(), usedValues.end());
		usedValues.erase(unique(usedValues.begin(), usedValues.end()), usedValues.end()); // no dups

		auto it = set_difference(range.begin(), range.end(), usedValues.begin(), usedValues.end(),
								 possibilities.begin());
		possibilities.resize(it - possibilities.begin());

		possibleValues[pos.row][pos.col] = move(possibilities);
	}
}

// Cross-reference rows and columns to find unique possible values for positions.
vector<pair<Position, int>> PossibleGrid::crossReference() const {
	vector<pair<Position, int>> pairs;
	for (int i = 0; i < grid->gridSize; ++i) {
		auto rowUnique = identifyUnique(i, true);  // ith row possibilities
		auto colUnique = identifyUnique(i, false); // ith col possibilities
		pairs.insert(pairs.end(), rowUnique.begin(), rowUnique.end());
		pairs.insert(pairs.end(), colUnique.begin(), colUnique.end());
	}
	return pairs; // results
}

// Identify unique values in a specified row/column
vector<pair<Position, int>> PossibleGrid::identifyUnique(int index, bool isRow) const {
	vector<int> possibilities;
	multimap<int, Position> valueMap; // map: quick lookup
	for (int i = 0; i < grid->gridSize; ++i)
	{
		auto &values = isRow ? possibleValues[index][i] : possibleValues[i][index];
		if (!values.empty()) {
			possibilities.insert(possibilities.end(), values.begin(), values.end());
			for (int val : values) {
				valueMap.insert({val, isRow ? Position(index, i) : Position(i, index)});
			}
		}
	}
	return filterUnique(possibilities, valueMap);
}

// Filter out unique values, grab their position from a map, and return unique pairs.
vector<pair<Position, int>> PossibleGrid::filterUnique(const vector<int> &values, 
														const multimap<int, Position> &map) const {
	vector<pair<Position, int>> uniquePairs; // in map
	auto filteredValues = values;

	sort(filteredValues.begin(), filteredValues.end());
	auto it = unique_copy(filteredValues.begin(), filteredValues.end(), filteredValues.begin());
	filteredValues.erase(it, filteredValues.end());

	for (int val : filteredValues) {
		if (map.count(val) == 1) { // appears once (unique)
			uniquePairs.push_back(make_pair(map.find(val)->second, val));
		}
	}
	return uniquePairs;
}

void PossibleGrid::print() const {
	for (const Position &pos : unsolvedPositions) {
		cout << pos.toString() << " : " << vectorToString(possibleValues[pos.row][pos.col]) << endl;
	}
}

string PossibleGrid::vectorToString(const vector<int> &vec) const{
	stringstream ss;
	copy(vec.begin(), vec.end(), ostream_iterator<int>(ss, " ")); // copy vec
	return ss.str().substr(0, ss.str().length() - 1); // remove last space
}
