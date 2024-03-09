/*
 * PossibleGird.cpp
 *
 *  Created on: 15 Jul 2015
 *      Author: Hongbo Tian
 *  Updated on: 8 Mar 2024
 * 		Editor: Soleil Cordray
 */

#include "PossibleGrid.h"
#include <cmath> // For std::sqrt in calculating section sizes dynamically

PossibleGrid::PossibleGrid()
{
}

// Analyzes the given grid to populate possible values for each cell. **** POTENTIALLY CHANGE ****
void PossibleGrid::Analysis(Grid &grid)
{
	clear();						 // Clears any existing data before starting the analysis.
	int gridSize = grid.dynamicSize; // Directly access the dynamicSize public member.

	mPossibleGrid.resize(gridSize, std::vector<std::vector<int>>(gridSize, std::vector<int>()));

	mUnsolvedPos = grid.getUnsolvedPos();

	for (auto &pos : mUnsolvedPos)
	{
		std::vector<int> invalid;

		// Collect numbers that are already used in the row, column, and section of the current position.
		std::vector<int> invalidRow = grid.getRow(pos.row);
		std::vector<int> invalidCol = grid.getCol(pos.col);
		std::vector<int> invalidSec = grid.getSectionByElement(pos.row, pos.col);

		// Combine all invalid numbers into a single vector.
		invalid.insert(invalid.end(), invalidRow.begin(), invalidRow.end());
		invalid.insert(invalid.end(), invalidCol.begin(), invalidCol.end());
		invalid.insert(invalid.end(), invalidSec.begin(), invalidSec.end());

		// Unique sort to remove duplicates.
		sort(invalid.begin(), invalid.end());
		invalid.erase(unique(invalid.begin(), invalid.end()), invalid.end());

		// Calculate possible values by excluding invalid numbers from 1 to gridSize.
		std::vector<int> possibleValues;
		for (int i = 1; i <= gridSize; ++i)
		{
			if (find(invalid.begin(), invalid.end(), i) == invalid.end())
			{
				possibleValues.push_back(i);
			}
		}

		// Ensure mPossibleGrid is correctly resized to accommodate dynamic grid sizes.
		if (mPossibleGrid.size() <= pos.row)
		{
			mPossibleGrid.resize(pos.row + 1);
		}
		if (mPossibleGrid[pos.row].size() <= pos.col)
		{
			mPossibleGrid[pos.row].resize(pos.col + 1);
		}

		// Update mPossibleGrid for the current position.
		mPossibleGrid[pos.row][pos.col] = possibleValues;
	}
}

// Performs cross-referencing to identify positions with unique possible values within rows, columns, and sections.
vector<pair<Pos, int>> PossibleGrid::crossRef()
{
	vector<pair<Pos, int>> pairs;		   // Stores positions and their determined values.
	int gridSize = grid.getDynamicSize();  // Assuming you have access to a Grid object named 'grid'.
	int sectionSize = std::sqrt(gridSize); // Calculate the size of each section.

	// Adjusted to consider dynamic grid size.
	for (int i = 0; i < gridSize; i++)
	{
		vector<int> rowCollection, colCollection;
		multimap<int, Pos> rowMap, colMap;

		for (int j = 0; j < gridSize; j++)
		{
			if (!mPossibleGrid[i][j].empty())
			{
				vector<int> possibleRow = mPossibleGrid[i][j];
				rowCollection.insert(rowCollection.end(), possibleRow.begin(), possibleRow.end());
				for (auto val : possibleRow)
				{
					rowMap.insert({val, Pos(i, j)});
				}
			}
			if (!mPossibleGrid[j][i].empty())
			{
				vector<int> possibleCol = mPossibleGrid[j][i];
				colCollection.insert(colCollection.end(), possibleCol.begin(), possibleCol.end());
				for (auto val : possibleCol)
				{
					colMap.insert({val, Pos(j, i)});
				}
			}
		}

		vector<int> uniqueRowValues = nonRepeat(rowCollection);
		for (auto val : uniqueRowValues)
		{
			pairs.push_back({rowMap.find(val)->second, val});
		}

		vector<int> uniqueColValues = nonRepeat(colCollection);
		for (auto val : uniqueColValues)
		{
			pairs.push_back({colMap.find(val)->second, val});
		}
	}

	// Cross-reference using dynamically calculated sections.
	for (int sRow = 0; sRow < gridSize; sRow += sectionSize)
	{
		for (int sCol = 0; sCol < gridSize; sCol += sectionSize)
		{
			vector<int> secCollection;
			multimap<int, Pos> secMap;

			for (int iRow = sRow; iRow < sRow + sectionSize; iRow++)
			{
				for (int iCol = sCol; iCol < sCol + sectionSize; iCol++)
				{
					if (!mPossibleGrid[iRow][iCol].empty())
					{
						vector<int> possibleSec = mPossibleGrid[iRow][iCol];
						secCollection.insert(secCollection.end(), possibleSec.begin(), possibleSec.end());
						for (auto val : possibleSec)
						{
							secMap.insert({val, Pos(iRow, iCol)});
						}
					}
				}
			}

			vector<int> uniqueSecValues = nonRepeat(secCollection);
			for (auto val : uniqueSecValues)
			{
				pairs.push_back({secMap.find(val)->second, val});
			}
		}
	}

	return pairs;
}

// Filters the input vector to return only non-repeating values.
vector<int> PossibleGrid::nonRepeat(vector<int> vec)
{
	sort(vec.begin(), vec.end());

	vector<int> nonRepeat; // This will store each number that appears exactly once in vec.
	// Iterate through sorted vec to collect non-repeating numbers.
	for (auto it = vec.begin(); it != vec.end();)
	{
		// Count the number of occurrences of *it.
		auto itNext = find_if(it, vec.end(), [it](int &value)
							  { return value != *it; });
		// If the number occurs exactly once, add it to nonRepeat.
		if (distance(it, itNext) == 1)
		{
			nonRepeat.push_back(*it);
		}
		// Move the iterator to the next distinct number.
		it = itNext;
	}

	return nonRepeat;
}

// Prints the possible values for unsolved positions.
void PossibleGrid::print()
{
	for (Pos &pos : mUnsolvedPos)
	{
		cout << pos.toString() << " : " << vecToString(mPossibleGrid[pos.row][pos.col]) << endl;
	}
}

// Clears the possible values grid, preparing for a new analysis.
void PossibleGrid::clear()
{
	mPossibleGrid.clear();
	mUnsolvedPos.clear();
}

// Converts a vector of integers to a space-separated string.
std::string PossibleGrid::vecToString(std::vector<int> &vec)
{
	stringstream ss;
	for (auto value : vec)
	{
		ss << value << " ";
	}
	return ss.str();
}
