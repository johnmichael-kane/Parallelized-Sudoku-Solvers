//========================================================================================
// Name        : Grid.cpp
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Edited 8 Mar 2024)
// Description : Manages the state and validation of a Sudoku grid. This class facilitates
//               reading from and writing to files, checking the legality and completeness
//               of the grid, and managing unsolved positions. It supports dynamic grid sizes,
//               making it suitable for standard and non-standard Sudoku puzzles.
//========================================================================================

#include "Grid.h"
#include <limits>

// Constructor initializing the grid with an empty name.
Grid::Grid() : gridSize(0), sectionSize(0), gridName("") {}

// Set value of a specific cell if position is within grid bounds.
void Grid::fill(const Pos &pos, int value) {
	if (pos.row >= 0 && pos.row < gridSize && pos.col >= 0 && pos.col < gridSize) {
		gridValues[pos.row][pos.col] = value;
	}
}

// Enforce Sudoku rule: no duplicate numbers in any row, column, or section.
// First, check whether numbers within each row/column are unique.
// Then, check whether numbers within each section are unique.
bool Grid::isLegal() const {
	for (int i = 0; i < gridSize; i++) {
		if (!isUnique(getRow(i)) || !isUnique(getCol(i))) {
			return false;
		}
	}

	for (int row = 0; row < gridSize; row += sectionSize) {
		for (int col = 0; col < gridSize; col += sectionSize) {
			if (!isUnique(getSection(row, col))) {
				return false;
			}
		}
	}

	return true;
}

// Verify if the grid completely filled with numbers 1 to gridSize in every row, column, and section.
// First, generate a sequence from 1 to gridSize named "identity".
// Then, determine whether sorted rows & columns match "identity" (i.e., are filled).
bool Grid::isComplete() const {
	if (!isLegal()) return false;

	vector<int> identity(gridSize);
	iota(identity.begin(), identity.end(), 1); 

	for (int i = 0; i < gridSize; i++) {
		auto row = getRow(i), col = getCol(i);
		sort(row.begin(), row.end());
		sort(col.begin(), col.end());
		if (row != identity || col != identity) {
			return false;
		}
	}
	return true;
}

// Check whether all numbers in a given vector are unique (no duplicates).
bool Grid::isUnique(const vector<int> &values) const {
	vector<int> temp(values);
	sort(temp.begin(), temp.end());
	auto last = unique(temp.begin(), temp.end());
	return last == temp.end();
}

//========================================================================================
// Getters
//========================================================================================

// If row (index) within bounds, return the values from that row; otherwise, return an empty vector
vector<int> Grid::getRow(int row) const {
	return row >= 0 && row < gridSize ? gridValues[row] : vector<int>();
}

// If col (index) within bounds, loop through grid rows and return all values from that column
std::vector<int> Grid::getCol(int col) const {
	vector<int> column;
	if (col >= 0 && col < gridSize)
	{
		for (const auto &row : gridValues)
		{
			column.push_back(row[col]);
		}
	}
	return column;
}

// Get top left section of given row and col (indices), and return all values from that section
vector<int> Grid::getSection(int row, int col) const {
	vector<int> section;
	int startRow = (row / sectionSize) * sectionSize;
	int startCol = (col / sectionSize) * sectionSize;
	for (int row = startRow; row < startRow + sectionSize; row++) {
		for (int col = startCol; col < startCol + sectionSize; col++) {
			section.push_back(gridValues[row][col]);
		}
	}
	return section;
}

// Loop through grid and return all unsolved positions within it
vector<Pos> Grid::getUnsolvedPositions() const {
	std::vector<Pos> unsolved;
	for (int row = 0; row < gridSize; row++) {
		for (int col = 0; col < gridSize; col++) {
			if (gridValues[row][col] == 0) {
				unsolved.emplace_back(row, col);
			}
		}
	}
	return unsolved;
}

//========================================================================================
// File
//========================================================================================

// Read and write to files 
bool Grid::read(const string &path)
{
	ifstream file(path);
	if (!file.is_open()) {
		return false;
	}

	// First line determines the size of the grid.
	file >> gridSize;
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip to the next line
	gridValues.resize(gridSize, vector<int>(gridSize));
	sectionSize = static_cast<int>(sqrt(gridSize));

	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			file >> gridValues[row][col];
		}
	}

	file.close();
	return true;
}

// Writes the current state of the Sudoku grid to a file.
bool Grid::write() const {
	ofstream file("ANS" + gridName);
	if (file.is_open()) {
        for (const auto& row : gridValues) {
            for (int num : row) {
                file << num << ' ';
            }
            file << '\n';
        }
        file.close();
        return true;
    }
    return false;
}

void Grid::print() const {
	for (const auto &row : gridValues) {
		for (int num : row) {
			cout << num << " ";
		}
		cout << endl;
	}
}
