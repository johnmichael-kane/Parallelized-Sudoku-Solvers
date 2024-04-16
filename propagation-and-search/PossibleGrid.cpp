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
vector<pair<Position, int>> PossibleGrid::crossReference() const
{
	vector<pair<Position, int>> pairs;
	for (int i = 0; i < gridSize; ++i)
	{
		auto rowUnique = identifyUniqueValues(i, true);	 // ith row possibilities
		auto colUnique = identifyUniqueValues(i, false); // ith col possibilities
		pairs.insert(pairs.end(), rowUnique.begin(), rowUnique.end());
		pairs.insert(pairs.end(), colUnique.begin(), colUnique.end());
	}
	return pairs;
}

// Cross-reference rows and columns and return unique possible value pairs (position & value).
vector<pair<Position, int>> PossibleGrid::identifyUniqueValues(int index, bool isRow) const
{
	vector<thread> workers;
	vector<pair<Position, int>> uniquePairs;
	mutex uniquePairsMutex;
	threadTasks = calculateThreadTasks(gridSize); // Ensure full coverage

	auto workerFunc = [&](size_t startIdx, size_t endIdx)
	{
		unordered_map<int, vector<Position>> valueMap;
		for (size_t j = startIdx; j < endIdx; ++j)
		{
			const auto &values = isRow ? possibleValues[index][j] : possibleValues[j][index];
			for (int val : values)
			{
				valueMap[val].push_back(isRow ? Position(index, j) : Position(j, index));
			}
		}

		vector<pair<Position, int>> localUniquePairs;
		for (auto &entry : valueMap)
		{
			if (entry.second.size() == 1)
			{ // Only add if unique
				localUniquePairs.emplace_back(entry.second.front(), entry.first);
			}
		}

		{
			lock_guard<mutex> lock(uniquePairsMutex);
			uniquePairs.insert(end(uniquePairs), begin(localUniquePairs), end(localUniquePairs));
		}
	};

	// Launch worker threads
	for (size_t i = 0; i < numThreads && i * threadTasks < gridSize; ++i)
	{
		size_t startIdx = i * threadTasks;
		size_t endIdx = std::min(startIdx + threadTasks, static_cast<size_t>(gridSize));
		workers.emplace_back(workerFunc, startIdx, endIdx);
	}

	// Join all threads
	for (auto &worker : workers)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}

	return uniquePairs;
}
