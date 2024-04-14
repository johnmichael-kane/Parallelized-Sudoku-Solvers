//========================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
// Description : Analyze a Sudoku grid and determine the possible values for each unsolved cell.
//========================================================================================

#include "PossibleGrid.h"

// Initialize null grid.
PossibleGrid::PossibleGrid() : grid(nullptr) {}

// If unsolved positions remain, return the first unsolved position.
Pos PossibleGrid::getFirstUnsolvedPosition()
{
	if (!unsolvedPositions.empty())
	{
		return unsolvedPositions.front();
	}
	else
	{
		return Pos(-1, -1);
	}
}

// Clear possible grid values (prepare for new analysis).
void PossibleGrid::clear()
{
	possibleGrid.clear();
	unsolvedPositions.clear();
}

// Find all invalid numbers within the row, column, and section intersecting the specified position.
// 1) Initialize invalids list (numbers that are already used) in a set (automatic non-duplicates).
// 2) Set up invalids list to be defined by an input vector and only include its non-zero elements. 
// 3) Add known invalids.
vector<int> PossibleGrid::invalidNumbers(Pos pos, Grid grid)
{
	std::set<int> invalids;

	auto addInvalids = [&](const vector<int> &source)
	{
		for (int num : source)
		{
			if (num != 0) invalids.insert(num);
		}
	};

	addInvalids(grid.getRow(pos.row));
	addInvalids(grid.getCol(pos.col));
	addInvalids(grid.getSection(pos.row, pos.col));

	return vector<int>(invalids.begin(), invalids.end());
}

// Analyze the given grid to populate possible values for each cell.
// 1) Resize possible grid according to grid size.
// 2) For each unsolved position in the grid, filter out numbers that are invalid (already used). 
// 3) Then, add the list of possible values (i.e., can use) for the unsolved positions. 
void PossibleGrid::Analysis(Grid &grid)
{
	clear();
	int gridSize = grid.gridSize;
	possibleGrid.resize(gridSize, vector<vector<int>>(gridSize));
	unsolvedPositions = grid.getUnsolvedPositions();

	for (const auto &pos : unsolvedPositions)
	{
		vector<int> invalids = invalidNumbers(pos, grid);
		vector<int> possibleValues;
		for (int i = 1; i <= gridSize; ++i)
		{
			if (find(invalids.begin(), invalids.end(), i) == invalids.end())
			{
				possibleValues.emplace_back(i);
			}
		}
		possibleGrid[pos.row][pos.col] = possibleValues;
	}
}

// Perform cross-referencing to find positions with truly unique possible values in their context.
// Iterate over all rows and columns of the grid.
// If a cell has only one possible value, check if unique in intersecting row, column, and section.
vector<pair<Pos, int>> PossibleGrid::crossReference()
{
	vector<pair<Pos, int>> uniquePositions;
	int gridSize = grid->gridSize;

	for (int row = 0; row < gridSize; row++)
	{
		for (int col = 0; col < gridSize; col++)
		{
			if (possibleGrid[row][col].size() == 1)
			{
				int value = possibleGrid[row][col].front();
				vector<int> rowValues = grid->getRow(row);
				vector<int> colValues = grid->getCol(col);
				vector<int> secValues = grid->getSection(row, col);

				if (grid->isUnique(rowValues) && grid->isUnique(colValues) && grid->isUnique(secValues))
				{
					uniquePositions.emplace_back(Pos(row, col), value);
				}
			}
		}
	}

	return uniquePositions;
}

// Convert vector of integers into a string separated by spaces.
std::string PossibleGrid::vectorsToString(std::vector<int> &vector)
{
	stringstream ss;
	for (auto value : vector)
	{
		ss << value << " ";
	}
	return ss.str();
}

// Print possible values for unsolved positions.
void PossibleGrid::print()
{
	for (const auto &pos : unsolvedPositions)
	{
		cout << pos.toString() << " : " << vectorsToString(possibleGrid[pos.row][pos.col]) << endl;
	}
}