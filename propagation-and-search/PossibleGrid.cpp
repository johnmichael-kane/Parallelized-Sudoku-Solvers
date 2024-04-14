//========================================================================================
// Name        : PossibleGrid.cpp
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
// Description : Analyze a Sudoku grid and determine the possible values for each unsolved cell.
//========================================================================================

#include "PossibleGrid.h"
#include <algorithm>
#include <iostream>
#include <sstream>

// Clear possible grid values (prepare for new analysis).
void PossibleGrid::clear() {
	possibleValues.clear();
	unsolvedPositions.clear();
}

// Analyze the given grid to populate possible values for each cell.
// 1) Resize possible grid according to grid size.
// 2) For each unsolved position in the grid, filter out numbers that are invalid (already used). 
// 3) Then, add the list of possible values (i.e., can use) for the unsolved positions. 
void PossibleGrid::Analysis() {
	clear();
	if (!grid)
	{
		std::cout << "Grid not set." << std::endl;
		return;
	}

	int gridSize = grid->gridSize;
	possibleValues.resize(gridSize, vector<vector<int>>(gridSize));

	unsolvedPositions = grid->getUnsolvedPositions();
	for (const auto &pos : unsolvedPositions) {
		vector<int> invalids = invalidNumbers(pos);
		vector<int> possibles;
		for (int num = 1; num <= gridSize; ++num) {
			if (find(invalids.begin(), invalids.end(), num) == invalids.end()) {
				possibles.push_back(num);
			}
		}
		possibleValues[pos.row][pos.col] = possibles;
		if (possibles.empty()) {
            std::cout << "No possible values for position (" << pos.row << ", " << pos.col << ")." << std::endl;
        }
	}
}

// Find all invalid numbers within the row, column, and section intersecting the specified position.
// 1) Initialize invalids list (numbers that are already used) in a set (automatic non-duplicates).
// 2) Set up invalids list to be defined by an input vector and only include its non-zero elements.
// 3) Add known invalids.
vector<int> PossibleGrid::invalidNumbers(const Pos &pos) const {
	std::set<int> invalids;
	auto addInvalids = [&](const vector<int> &source) {
		for (int num : source) {
			if (num != 0) invalids.insert(num);
		}
	};

	addInvalids(grid->getRow(pos.row));
	addInvalids(grid->getCol(pos.col));
	addInvalids(grid->getSection(pos.row, pos.col));

	return vector<int>(invalids.begin(), invalids.end());
}

// Perform cross-referencing to find positions with truly unique possible values in their context.
// Iterate over all rows and columns of the grid.
// If a cell has only one possible value, check if unique in intersecting row, column, and section.
vector<pair<Pos, int>> PossibleGrid::crossReference() {
	vector<pair<Pos, int>> uniquePositions;
	for (int row = 0; row < grid->gridSize; ++row)
	{
		for (int col = 0; col < grid->gridSize; ++col)
		{
			const auto &possibles = possibleValues[row][col];
			if (possibles.size() == 1)
			{
				bool isUnique = true;
				// Additional check to ensure the value is indeed unique in its context
				for (int otherRow = 0; otherRow < grid->gridSize; ++otherRow)
				{
					if (otherRow != row && !possibleValues[otherRow][col].empty() &&
						std::find(possibleValues[otherRow][col].begin(), possibleValues[otherRow][col].end(), possibles.front()) != possibleValues[otherRow][col].end())
					{
						isUnique = false;
						break;
					}
				}
				for (int otherCol = 0; otherCol < grid->gridSize && isUnique; ++otherCol)
				{
					if (otherCol != col && !possibleValues[row][otherCol].empty() &&
						std::find(possibleValues[row][otherCol].begin(), possibleValues[row][otherCol].end(), possibles.front()) != possibleValues[row][otherCol].end())
					{
						isUnique = false;
						break;
					}
				}
				if (isUnique)
				{
					uniquePositions.emplace_back(Pos(row, col), possibles.front());
				}
			}
		}
	}
	return uniquePositions;
}

// Converting vector of integers to a string separated by spaces
string PossibleGrid::vectorsToString(const vector<int> &vec) const {
	std::stringstream ss;
	for (int value : vec)
	{
		ss << value << " ";
	}
	// Removing the extra space at the end of the string
	string result = ss.str();
	result.pop_back();
	return result;
}

// Print possible values for unsolved positions
void PossibleGrid::print() const {
	for (const auto &pos : unsolvedPositions) {
		cout << pos.toString() << " : " << vectorsToString(possibleValues[pos.row][pos.col]) << endl;
	}
}