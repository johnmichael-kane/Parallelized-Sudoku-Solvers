//==================================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Manage possible values for Sudoku grid cells to assist in solving Sudoku puzzles.
//==================================================================================================

#include "PossibleGrid.h"
using namespace std;
mutex mtx;

// PARALLELIZE: reduce overall time it takes to compute possible values for each unsolved position
void PossibleGrid::analyzeMoves(const Grid &grid) {
	if (!gridSize) return;

	clear();
	possibleValues.resize(gridSize, vector<vector<int>>(gridSize));
	unsolvedPositions = grid.getUnsolvedPositions();

	vector<int> range(gridSize);
	iota(range.begin(), range.end(), 1); // Initialize possible values range

	vector<future<void>> futures;
	size_t threadTasks = calculateThreadTasks(unsolvedPositions.size());
	for (size_t i = 0; i < numThreads; ++i) {
		// Launch a task for each position asynchronously
		futures.push_back(async(launch::async, [&, i] {
			size_t start = i * threadTasks;
			size_t end = min(start + threadTasks, unsolvedPositions.size());

			for (size_t j = start; j < end; ++j) {
				const auto &position = unsolvedPositions[j];
				auto usedValues = getUsedValues(grid, position);

				vector<int> possibilities;
				set_difference(range.begin(), range.end(), usedValues.begin(), usedValues.end(),
							   back_inserter(possibilities));
				possibleValues[position.row][position.col] = std::move(possibilities); 
			} 
		}));
	}

	// Wait for all futures to complete
	for (auto &fut : futures) {
		fut.get();
	}
}

vector<int> PossibleGrid::getUsedValues(const Grid &grid, const Position &position)
{
	set<int> valueSet;
	auto appendValues = [&](const vector<int> &src) {
		valueSet.insert(src.begin(), src.end());
	};

	appendValues(grid.getSection(position.row, position.col));
	appendValues(grid.getRow(position.row));
	appendValues(grid.getCol(position.col));

	// Remove duplicates via a set
	return vector<int>(valueSet.begin(), valueSet.end());

}

// Cross-reference rows and columns and return unique possible value pairs (position & value).
vector<pair<Position, int>> PossibleGrid::crossReference() const {
	vector<pair<Position, int>> pairs;
	vector<future<vector<pair<Position, int>>>> futures;

	// Launch threads for rows and columns in parallel
	for (int i = 0; i < gridSize; ++i) {
		futures.push_back(async(launch::async, &PossibleGrid::identifyUniqueValues, this, i, true));  // For rows
		futures.push_back(async(launch::async, &PossibleGrid::identifyUniqueValues, this, i, false)); // For columns
	}

	// Collect results from futures
	for (auto &fut : futures) {
		vector<pair<Position, int>> unique = fut.get();
		pairs.insert(pairs.end(), unique.begin(), unique.end());
	}

	return pairs;
}

//
// Parallelization - everything slightly worse except medium & hard 25 (those are slightly better)
// goal: collect all outputs efficiently, minimize shared access & synchronization (collect at end)
// accumulate results in shared storage instead of locking

vector<pair<int, Position>> workerFunction(const vector<vector<vector<int>>> &possibleValues, int index, bool isRow, int start, int end) {
	vector<pair<int, Position>> results;
	for (int i = start; i < end; ++i) {
		if ((isRow && index >= possibleValues.size()) || (!isRow && i >= possibleValues.size()) ||
			(isRow && i >= possibleValues[index].size()) || (!isRow && index >= possibleValues[i].size())) {
			continue; // Skip this iteration if indices are out of range
		}

		const vector<int> &values = isRow ? possibleValues[index][i] : possibleValues[i][index];
		for (int val : values) {
			Position pos = isRow ? Position(index, i) : Position(i, index);
			results.emplace_back(val, pos);
		}
	}
	return results;
}

// Identify unique values in a specified row/column
// Improved Aggregation Strategy: Use a thread-safe method to aggregate results from multiple threads to avoid potential delays and manage synchronization efficiently.
vector<pair<Position, int>> PossibleGrid::identifyUniqueValues(int index, bool isRow) const {
	vector<future<vector<pair<int, Position>>>> futures;
	vector<pair<Position, int>> uniquePairs;
	int numTasks = calculateThreadTasks(static_cast<size_t>(gridSize));

	for (int i = 0; i < numThreads; ++i) {
		int start = i * numTasks;
		int end = (i == numThreads - 1) ? gridSize : start + numTasks;
		futures.push_back(async(launch::async, workerFunction, cref(possibleValues), index, isRow, start, end));
	}

	map<int, vector<Position>> valueMap;
	for (auto &fut : futures) {
		auto result = fut.get();
		for (const auto &[val, pos] : result)
		{
			valueMap[val].push_back(pos);
		}
	}

	// Extract unique values and positions
	for (const auto &[val, positions] : valueMap) {
		if (positions.size() == 1)
		{
			uniquePairs.emplace_back(positions.front(), val);
		}
	}

	return uniquePairs;
}