//==================================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Manage possible values for Sudoku grid cells to assist in solving Sudoku puzzles.
//==================================================================================================

#include "PossibleGrid.h"

using namespace std;

vector<int> PossibleGrid::getUsedValues(const Grid &grid, const Position &position)
{
	vector<int> values;
	auto appendValues = [&](const vector<int> &src)
	{
		values.insert(values.end(), src.begin(), src.end());
	};

	appendValues(grid.getSection(position.row, position.col));
	appendValues(grid.getRow(position.row));
	appendValues(grid.getCol(position.col));

	// Remove duplicates
	sort(values.begin(), values.end());
	values.erase(unique(values.begin(), values.end()), values.end());
	return values;
}

// PARALLELIZE: reduce overall time it takes to compute possible values for each unsolved position
void PossibleGrid::analyzeMoves(const Grid &grid)
{
	if (this->grid == nullptr) return;

	clear();
	possibleValues.resize(gridSize, vector<vector<int>>(gridSize));
	unsolvedPositions = grid.getUnsolvedPositions();

	// Define number range [1 ... gridSize]
	vector<int> range(gridSize);
	iota(range.begin(), range.end(), 1);

	vector<future<void>> futures; // asynchronous execution
	size_t threadTasks = calculateThreadTasks(unsolvedPositions.size());

	for (size_t i = 0; i < numThreads; ++i)
	{
		// Launch a task for each position asynchronously
		futures.push_back(async(launch::async, [&, i]
								{
			size_t start = i * threadTasks;
			size_t end = min(start + threadTasks, unsolvedPositions.size());

			for (size_t j = start; j < end; ++j) {
				const auto &position = unsolvedPositions[j];
				vector<int> usedValues = getUsedValues(grid, position);

				vector<int> possibilities;
				set_difference(range.begin(), range.end(), usedValues.begin(), usedValues.end(),
							   back_inserter(possibilities));
				possibleValues[position.row][position.col] = std::move(possibilities); 
			} }));
	}

	// Wait for all futures to complete
	for (auto &fut : futures)
	{
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

void workerFunction(const vector<vector<vector<int>>> &possibleValues, int index, bool isRow, int start, int end,
					map<int, vector<Position>> &localMap, mutex &mtx)
{
	for (int i = start; i < end; ++i)
	{
		vector<int> localValues; // Local storage for thread-safe operation
		{
			lock_guard<mutex> lock(mtx); // Protect reading from shared vector
			if (index < possibleValues[i].size() && isRow
					? index < possibleValues.size() && i < possibleValues[index].size()
					: i < possibleValues.size() && index < possibleValues[i].size())
			{
				const vector<int> &values = isRow ? possibleValues[index][i] : possibleValues[i][index];
				localValues = values; // Make a local copy to work with
			}
			else
			{
				continue; // Skip this iteration if indices are out of range
			}
		} // Mutex unlocks here

		for (int val : localValues)
		{
			Position pos = isRow ? Position(index, i) : Position(i, index);
			lock_guard<mutex> lock(mtx); // Thread-safe insertion into map
			localMap[val].push_back(pos);
		}
	}
}

// Identify unique values in a specified row/column
vector<pair<Position, int>> PossibleGrid::identifyUniqueValues(int index, bool isRow) const
{
	vector<thread> threads;
	vector<map<int, vector<Position>>> threadMaps(numThreads);
	mutex mtx;

	// Divide work among threads
	threadTasks = calculateThreadTasks(static_cast<size_t>(gridSize));
	for (int i = 0; i < numThreads; ++i)
	{
		int start = i * threadTasks;
		int end = (i == numThreads - 1) ? gridSize : start + threadTasks;
		threads.emplace_back(workerFunction, ref(possibleValues), index, isRow, start, end, ref(threadMaps[i]), ref(mtx));
	}

	// Wait for all threads to complete
	for (auto &th : threads)
	{
		th.join();
	}

	// Merge results
	map<int, vector<Position>> mergedMap;
	for (auto &tm : threadMaps)
	{
		for (auto &pair : tm)
		{
			mergedMap[pair.first].insert(mergedMap[pair.first].end(), pair.second.begin(), pair.second.end());
		}
	}

	// Extract unique values
	vector<pair<Position, int>> uniquePairs;
	for (auto &pair : mergedMap)
	{
		if (pair.second.size() == 1)
		{
			uniquePairs.emplace_back(pair.second.front(), pair.first);
		}
	}

	return uniquePairs;
}