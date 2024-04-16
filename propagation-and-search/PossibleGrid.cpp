//==================================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Manage possible values for Sudoku grid cells to assist in solving Sudoku puzzles.
//==================================================================================================

#include "PossibleGrid.h"

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

	vector<future<void>> futures; // asynchronous execution
	threadTasks = calculateThreadTasks(unsolvedPositions.size());

	for (size_t i = 0; i < numThreads; ++i) {
		// Launch a task for each position asynchronously
		futures.push_back(async(launch::async, [&, i] {
			size_t start = i * threadTasks;
			size_t end = min(start + threadTasks, unsolvedPositions.size());

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
			} 
		}));
	}

	// Wait for all futures, collect results
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
	vector<future<vector<pair<Position, int>>>> futures;
	threadTasks = calculateThreadTasks(gridSize);

	for (size_t i = 0; i < numThreads; ++i) {
		futures.push_back(async(launch::async, [&, i] {
			size_t start = i * threadTasks;
			size_t end = min(start + threadTasks, static_cast<size_t>(gridSize));

			multimap<int, Position> valueMap;
			for (size_t j = start; j < end; ++j) {
				const vector<int> &values = isRow ? possibleValues[index][j] : possibleValues[j][index];
				for (int val : values) {
					valueMap.emplace(val, isRow ? Position(index, j) : Position(j, index));
				}
			}

			vector<pair<Position, int>> localUniquePairs;
			for (auto it = valueMap.begin(); it != valueMap.end(); ++it) { 
				if (valueMap.count(it->first) == 1) {
					localUniquePairs.push_back(make_pair(it->second, it->first)); // position, value
				}
			}
			return localUniquePairs;
		}));
	}

	// Wait for all futures, aggregate results
	vector<pair<Position, int>> uniquePairs;
	for (auto &fut : futures) {
		auto localPairs = fut.get();
		uniquePairs.insert(uniquePairs.end(), localPairs.begin(), localPairs.end());
	}

	return uniquePairs;
}
