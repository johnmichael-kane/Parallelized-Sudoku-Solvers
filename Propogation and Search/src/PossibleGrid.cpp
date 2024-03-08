/*
 * PossibleGird.cpp
 *
 *  Created on: 15 Jul 2015
 *      Author: Hongbo Tian
 *  Updated on: 8 Mar 2024
 * 		Editor: Soleil Cordray
 */

#include "PossibleGrid.h"

PossibleGrid::PossibleGrid()
{
}

// Analyzes the given grid to populate possible values for each cell.
void PossibleGrid::Analysis(Grid grid)
{
	clear(); // Clears any existing data before starting the analysis.

	// Retrieve unsolved positions from the grid.
	mUnsolvedPos = grid.getUnsolvedPos();
	vector<Pos>::iterator it;

	for (auto it = mUnsolvedPos.begin(); it != mUnsolvedPos.end(); ++it)
	{
		vector<int> invalid;

		// Collect numbers that are already used in the row, column, and section of the current position.
		// (i.e., collect invalid numbers)
		vector<int> invalidRow = grid.getRow(it->row);
		vector<int> invalidCol = grid.getCol(it->col);
		vector<int> invalidSec = grid.getSectionByElement(it->row, it->col);

		// Combine all invalid numbers into a single vector.
		invalid.insert(invalid.end(), invalidRow.begin(), invalidRow.end());
		invalid.insert(invalid.end(), invalidCol.begin(), invalidCol.end());
		invalid.insert(invalid.end(), invalidSec.begin(), invalidSec.end());

		// Append identity (all numbers from 1 to Grid::MAX) to the vector.
		for (int i = 0; i < Grid::MAX; i++)
		{
			invalid.push_back(i + 1);
		}

		// Filter to keep only the numbers that do not repeat, indicating possible values for this cell.
		// (i.e., keep only the non-repeating values)
		mPossibleGrid[it->row][it->col] = nonRepeat(invalid);
	}
}

// Performs cross-referencing to identify positions with unique possible values within rows, columns, and sections.
vector<pair<Pos, int>> PossibleGrid::crossRef()
{
	vector<pair<Pos, int>> pairs; // Stores positions and their determined values.

	// Process rows and columns.
	for (int i = 0; i < Grid::MAX; i++)
	{
		vector<int> rowCollection, colCollection;
		multimap<int, Pos> rowMap, colMap;

		// Setup possible values arrays and mapping for rows and columns.
		for (int j = 0; j < Grid::MAX; j++)
		{
			// Same row
			if (!mPossibleGrid[i][j].empty()) // OLD: mPossibleGrid[i][j].size() > 0
			{
				// Collect the possible values
				vector<int> possibleRow = mPossibleGrid[i][j];
				rowCollection.insert(rowCollection.end(), possibleRow.begin(), possibleRow.end());

				// Add to lookup map
				vector<int>::iterator it;
				for (it = possibleRow.begin(); it != possibleRow.end(); it++)
				{
					rowMap.insert(make_pair(*it, Pos(i, j)));
				}
				/* POTENTIAL OPTIMIZATION: (add to lookup map)
				for (auto it : possibleRow)
				{
					rowMap.insert(make_pair(it, Pos(i, j)));
				} */
			}
			// Same column
			if (!mPossibleGrid[j][i].empty()) // OLD: mPossibleGrid[j][i].size() > 0
			{
				// Collect the possible values
				vector<int> possibleCol = mPossibleGrid[j][i];
				colCollection.insert(colCollection.end(), possibleCol.begin(), possibleCol.end());

				// Add to lookup map
				vector<int>::iterator it;
				for (it = possibleCol.begin(); it != possibleCol.end(); it++)
				{
					colMap.insert(make_pair(*it, Pos(j, i)));
				}
				/* POTENTIAL OPTIMIZATION: (add to lookup map)
				for (auto it : possibleCol)
				{
					colMap.insert(make_pair(it, Pos(j, i)));
				} */
			}
		}

		// Process non-repeating values for rows and columns to identify unique solutions.
		vector<int> nonRepeatingRow = nonRepeat(rowCollection);
		vector<int> nonRepeatingCol = nonRepeat(colCollection);

		// Add the pos and value of that element to the returning vector
		vector<int>::iterator rowIt, colIt;
		for (rowIt = nonRepeatingRow.begin(); rowIt != nonRepeatingRow.end();
			 rowIt++)
		{
			pairs.push_back(make_pair(rowMap.find(*rowIt)->second, *rowIt));
		}

		for (colIt = nonRepeatingCol.begin(); colIt != nonRepeatingCol.end();
			 colIt++)
		{
			pairs.push_back(make_pair(colMap.find(*colIt)->second, *colIt));
		}
		/* POTENTIAL OPTIMIZATION: (add pos & val)
		for (auto rowIt : nonRepeatingRow)
		{
			pairs.push_back(make_pair(rowMap.find(rowIt)->second, rowIt));
		}
		for (auto colIt : nonRepeatingCol)
		{
			pairs.push_back(make_pair(colMap.find(colIt)->second, colIt));
		} */
	}

	// Cross-reference using sections to find unique solutions. **** SIZE ****
	for (int sRow = 0; sRow < 3; sRow++)
	{
		for (int sCol = 0; sCol < 3; sCol++)
		{
			// Section **** SIZE? ****
			vector<int> possibleSec;
			multimap<int, Pos> secMap;

			// Process each section.
			for (int iRow = 0; iRow < 3; iRow++)
			{
				for (int iCol = 0; iCol < 3; iCol++)
				{
					int row = sRow * 3 + iRow;
					int col = sCol * 3 + iCol;
					if (!mPossibleGrid[row][col].empty()) // OLD: mPossibleGrid[row][col].size() > 0
					{
						// Extract the possible values
						vector<int> possible = mPossibleGrid[row][col];
						// Add all the possible values in that section
						possibleSec.insert(possibleSec.end(), possible.begin(), possible.end());
						
						// Construct the Lookup map
						for (vector<int>::iterator it = possible.begin();
							 it != possible.end(); it++)
						{
							secMap.insert(make_pair(*it, Pos(row, col)));
						}

						/* POTENTIAL OPTIMIZATION:
						for (auto it : possible)
						{
							secMap.insert(make_pair(it, Pos(row, col)));
						} */
					}
				}
			}
			// Find unique solutions within each section. (Section Code)
			vector<int> nonRepeatingSec = nonRepeat(possibleSec);
			for (vector<int>::iterator it = nonRepeatingSec.begin();
				 it != nonRepeatingSec.end(); it++)
			{
				pairs.push_back(make_pair(secMap.find(*it)->second, *it));
			}

			/* POTENTIAL OPTIMIZATION:
			for (auto it : nonRepeatingSec)
			{
				pairs.push_back(make_pair(secMap.find(it)->second, it));
			} */
		}
	}

	return pairs;
}

// Filters the input vector to return only non-repeating values.
vector<int> PossibleGrid::nonRepeat(vector<int> vec)
{
	// First, sort the input vector. This makes it easier to find repeating values
	// because identical values will be adjacent to each other.
	sort(vec.begin(), vec.end());

	// Append a sentinel value to the end of the vector.
	// This is a common technique to simplify the logic of loops that process sequences of elements
	// by ensuring that the last "real" element is processed correctly without adding special-case code outside the loop.
	vec.push_back(0);

	// Initialize variables:
	// 'buffer' holds the current value being compared against subsequent values.
	// 'counter' tracks the number of times the current value ('buffer') has been repeated.
	int buffer = vec.at(0), counter = 0;

	// This vector will hold the non-repeating values found in 'vec'.
	vector<int> nonRepeat;

	// Iterate through the vector starting from the second element (since we already read the first element into 'buffer').
	for (auto it = vec.begin() + 1; it != vec.end(); ++it)
	{
		if (*it != buffer && counter == 0)
		{
			// If the current value pointed by the iterator is different from 'buffer'
			// and 'counter' is 0 (meaning 'buffer' has not been repeated),
			// then 'buffer' is a unique value and we add it to 'nonRepeat'.
			nonRepeat.push_back(buffer);

			// Reset 'counter' to 0 and update 'buffer' to the current value,
			// preparing to check the next set of values.
			counter = 0;
			buffer = *it;
		}
		else if (*it != buffer)
		{
			// If the current value is different from 'buffer' but 'counter' is not 0,
			// it means we have encountered a new value after a sequence of repeating values.
			// We reset 'counter' and update 'buffer' to the current value,
			// but we don't add 'buffer' to 'nonRepeat' because it was repeated.
			buffer = *it;
			counter = 0;
		}
		else
		{
			// If the current value is the same as 'buffer',
			// increment 'counter' to indicate a repetition.
			counter++;
		}
	}
	// Return the vector containing only non-repeating values from the original input.
	return nonRepeat;
}

// Prints the possible values for unsolved positions.
void PossibleGrid::print()
{
	// Create an iterator to loop through the vector of unsolved positions.
	vector<Pos>::iterator it;

	// Iterate over all unsolved positions stored in 'mUnsolvedPos'.
	for (it = mUnsolvedPos.begin(); it != mUnsolvedPos.end(); it++)
	{
		// Print the position (row and column) of the unsolved cell to the console.
		// The 'toString()' method of 'Pos' is assumed to return a string representation of the position.
		cout << it->toString() << flush; // Ensure immediate output with 'flush'.

		// Print a separator (":") to visually distinguish between the position and its possible values.
		cout << " : " << flush;

		// Print the possible values for this position.
		cout << vecToString(mPossibleGrid[it->row][it->col]) << endl;
	}
	/* POTENTIAL OPTIMIZATION:
	for (auto it = mUnsolvedPos.begin(); it != mUnsolvedPos.end(); ++it)
	{
		cout << it->toString() << " : " << vecToString(mPossibleGrid[it->row][it->col]) << endl;
	} */
}

// Clears the possible values grid, preparing for a new analysis.
void PossibleGrid::clear()
{
	for (int i = 0; i < Grid::MAX; i++)
	{
		for (int j = 0; j < Grid::MAX; j++)
		{
			vector<int> dummy;
			mPossibleGrid[i][j] = dummy;
		}
	}

	/* POTENTIAL OPTIMIZATION:
	for (int i = 0; i < Grid::MAX; i++)
	{
		for (int j = 0; j < Grid::MAX; j++)
		{
			mPossibleGrid[i][j].clear();
		}
	} */
}

// Converts a vector of integers to a space-separated string.
string PossibleGrid::vecToString(vector<int> vec)
{
	stringstream ss;
	for (vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		ss << *it;
		ss << " ";
	}

	/* POTENTIAL OPTIMIZATION:
	for (auto it : vec)
	{
		ss << it << " ";
	} */

	return ss.str();
}