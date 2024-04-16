//==================================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Manage possible values for Sudoku grid cells to assist in solving Sudoku puzzles.
//==================================================================================================

#include "PossibleGrid.h"
#include <mutex>
#include <future>

using namespace std;

// PARALLELIZE: reduce overall time it takes to compute possible values for each unsolved position
void PossibleGrid::analyzeMoves(const Grid &grid) {
	if (this->grid == nullptr) return;

	clear();
	possibleValues.resize(gridSize, vector<vector<int>>(gridSize));
	unsolvedPositions = grid.getUnsolvedPositions();

	// Define number range [1 ... gridSize]
	vector<int> range(gridSize);
	iota(range.begin(), range.end(), 1);

	const size_t numThreads = thread::hardware_concurrency();
	size_t batchSize = unsolvedPositions.size() / numThreads + (unsolvedPositions.size() % numThreads != 0);
	vector<future<void>> futures; // asynchronous execution

	for (size_t i = 0; i < numThreads; ++i) {
		// Launch a task for each position asynchronously
		futures.push_back(async(launch::async, [&, i] {
			size_t start = i * batchSize;
            size_t end = min(start + batchSize, unsolvedPositions.size());

			for (size_t j = start; j < end; ++j) {
				const auto &position = unsolvedPositions[j];
				vector<int> usedValues;

				const auto &usedSectionValues = grid.getSection(position.row, position.col);
				const auto &usedRowValues = grid.getRow(position.row);
				const auto &usedColValues = grid.getCol(position.col);

				usedValues.insert(usedValues.end(), usedSectionValues.begin(), usedSectionValues.end());
				usedValues.insert(usedValues.end(), usedRowValues.begin(), usedRowValues.end());
				usedValues.insert(usedValues.end(), usedColValues.begin(), usedColValues.end());

				// Remove duplicates
				sort(usedValues.begin(), usedValues.end());
				usedValues.erase(unique(usedValues.begin(), usedValues.end()), usedValues.end());

				vector<int> possibilities;
				set_difference(range.begin(), range.end(), usedValues.begin(), usedValues.end(),
							back_inserter(possibilities));
				possibleValues[position.row][position.col] = std::move(possibilities); 
			} }));
	}

	// Wait for all futures to complete
	for (auto &fut : futures) {
		fut.get();
	}
}

// Cross-reference rows and columns and return unique possible value pairs (position & value).
vector<pair<Position, int>> PossibleGrid::crossReference() const {
	vector<pair<Position, int>> pairs;
	for (int i = 0; i < gridSize; ++i) {
		auto rowUnique = identifyUniqueValues(i, true);  // ith row possibilities
		auto colUnique = identifyUniqueValues(i, false); // ith col possibilities
		pairs.insert(pairs.end(), rowUnique.begin(), rowUnique.end());
		pairs.insert(pairs.end(), colUnique.begin(), colUnique.end());
	}
	return pairs;
}

// Identify unique values in a specified row/column
vector<pair<Position, int>> PossibleGrid::identifyUniqueValues(int index, bool isRow) const {
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
