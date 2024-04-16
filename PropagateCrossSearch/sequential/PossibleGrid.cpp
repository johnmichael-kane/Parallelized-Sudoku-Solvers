//==================================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Manage possible values for Sudoku grid cells to assist in solving Sudoku puzzles.
//==================================================================================================

#include "PossibleGrid.h"
using namespace std;

void PossibleGrid::analyzeMoves(const Grid &grid)
{
	if (!gridSize)
		return;

	clear();
	possibleValues.resize(gridSize, vector<vector<int>>(gridSize));
	unsolvedPositions = grid.getUnsolvedPositions();

	vector<int> range(gridSize);
	iota(range.begin(), range.end(), 1); // Initialize possible values range

	for (const auto &position : unsolvedPositions)
	{
		auto usedValues = getUsedValues(grid, position);

		vector<int> possibilities;
		set_difference(range.begin(), range.end(), usedValues.begin(), usedValues.end(),
					   back_inserter(possibilities));
		possibleValues[position.row][position.col] = std::move(possibilities);
	}
}

vector<int> PossibleGrid::getUsedValues(const Grid &grid, const Position &position)
{
	set<int> valueSet;
	auto appendValues = [&](const vector<int> &src)
	{
		valueSet.insert(src.begin(), src.end());
	};

	appendValues(grid.getSection(position.row, position.col));
	appendValues(grid.getRow(position.row));
	appendValues(grid.getCol(position.col));

	// Remove duplicates via a set
	return vector<int>(valueSet.begin(), valueSet.end());
}

vector<pair<Position, int>> PossibleGrid::crossReference() const
{
	vector<pair<Position, int>> pairs;

	for (int i = 0; i < gridSize; ++i)
	{
		// For rows
		auto rowPairs = identifyUniqueValues(i, true);
		pairs.insert(pairs.end(), rowPairs.begin(), rowPairs.end());

		// For columns
		auto colPairs = identifyUniqueValues(i, false);
		pairs.insert(pairs.end(), colPairs.begin(), colPairs.end());
	}

	return pairs;
}

vector<pair<Position, int>> PossibleGrid::identifyUniqueValues(int index, bool isRow) const
{
	vector<pair<int, Position>> results = workerFunction(possibleValues, index, isRow, 0, gridSize);

	map<int, vector<Position>> valueMap;
	for (const auto &[val, pos] : results)
	{
		valueMap[val].push_back(pos);
	}

	vector<pair<Position, int>> uniquePairs;
	// Extract unique values and positions
	for (const auto &[val, positions] : valueMap)
	{
		if (positions.size() == 1)
		{
			uniquePairs.emplace_back(positions.front(), val);
		}
	}

	return uniquePairs;
}

vector<pair<int, Position>> PossibleGrid::workerFunction(const vector<vector<vector<int>>> &possibleValues, 
														 int index, bool isRow, int start, int end) const {
	vector<pair<int, Position>> results;
	for (int i = start; i < end; ++i)
	{
		if ((isRow && index >= possibleValues.size()) || (!isRow && i >= possibleValues.size()) ||
			(isRow && i >= possibleValues[index].size()) || (!isRow && index >= possibleValues[i].size()))
		{
			continue; // Skip this iteration if indices are out of range
		}

		const vector<int> &values = isRow ? possibleValues[index][i] : possibleValues[i][index];
		for (int val : values)
		{
			Position pos = isRow ? Position(index, i) : Position(i, index);
			results.emplace_back(val, pos);
		}
	}
	return results;
}
