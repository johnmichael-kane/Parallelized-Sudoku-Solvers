//==================================================================================================
// Name        : Grid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Implement Grid class by manipulating grid states, checking legality/completeness,
//				 and handling file-based input/output operations for Sudoku puzzles.
//==================================================================================================

#include "Grid.h"

// Fill cell with specified value at specified position (if in bounds).
void Grid::fill(const Position &pos, int value) {
	if (pos.row >= 0 && pos.row < gridSize && pos.col >= 0 && pos.col < gridSize) { 
		grid[pos.row][pos.col] = value;
	}
}

//
// Conditions
//

// Verify whether rows, columns, and sections follow Sudoku rules (no duplicates).
bool Grid::isLegal() {
	auto isUnique = [](const std::vector<int> &vec) -> bool {
		std::vector<int> filteredVector;
		std::copy_if(vec.begin(), vec.end(), std::back_inserter(filteredVector), [](int v)
					 { return v != 0; }); // remove zeroes
		std::sort(filteredVector.begin(), filteredVector.end());
		auto last = std::unique(filteredVector.begin(), filteredVector.end());
		return last == filteredVector.end(); // unique: nothing removed
	};

	for (int i = 0; i < gridSize; ++i) {
		if (!isUnique(getRow(i)) || !isUnique(getCol(i)) 
			|| !isUnique(getSection(i / sectionSize, i % sectionSize))) // ith row, ith col
			return false;
	}

	return true;
}

// Verify that no rows contain empty cells (if empty cells exist, return false).
bool Grid::isComplete() {
	return isLegal() && std::none_of(grid.begin(), grid.end(), [](const std::vector<int> &row)
						{ return std::any_of(row.begin(), row.end(), [](int cell)
						{ return cell == 0; }); });
}

//
// File Operations
//

// Read puzzle file: grid size = first line, then read everything else into grid.
// Calculate section size (square root of grid size) and resize the grid according to grid size.
bool Grid::read(const string &filename)
{
	ifstream file(filename);
	if (!file.is_open())
		return false;

	puzzle = filename;
	file >> gridSize;
	sectionSize = int(sqrt(gridSize));
	grid.resize(gridSize, vector<int>(gridSize));

	for (auto &row : grid) // modify grid
		for (int &cell : row)
			file >> cell;

	file.close();
	return true;
}

// Write grid into a new "answer" file.
bool Grid::write() const
{
	string filename = "ANS" + puzzle;
	ofstream file(filename);
	if (!file.is_open())
		return false;

	for (const auto &row : grid)
	{ // output grid
		for (int cell : row)
			file << cell << " ";
		file << "\n";
	}

	file.close();
	return true;
}

// Print grid to console.
void Grid::print() const
{
	for (const auto &row : grid)
	{
		for (int cell : row)
			cout << cell << " ";
		cout << "\n";
	}
}

//
// Retrieval
//

// Return row (vec) of grid at specified index if within bounds (return empty vector otherwise).
const vector<int> Grid::getRow(int row) const {
	return (row >= 0 && row < gridSize) ? grid[row] : vector<int>(); 
}

// Return column (vec) of grid at specified index if within bounds (return empty vector otherwise).
const vector<int> Grid::getCol(int col) const
{
	if (col < 0 || col >= gridSize) return vector<int>();

	vector<int> column(gridSize);
	for (int i = 0; i < gridSize; ++i) {
		column[i] = grid[i][col];
	}
	return column;
}

// Return section (vec) of grid, starting from the top leftmost corner.
const vector<int> Grid::getSection(int row, int col) const {
	vector<int> section;
	int startRow = getSectionStart(row), startCol = getSectionStart(col); 
	for (int r = startRow; r < startRow + sectionSize; ++r) {
		for (int c = startCol; c < startCol + sectionSize; ++c) {
			section.push_back(grid[r][c]); 
		}
	}
	return section;
}


const vector<Position> Grid::getUnsolvedPositions() const {
	vector<Position> unsolved;
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			if (grid[row][col] == 0) {
				unsolved.emplace_back(row, col); // construct directly
			}
		}
	}
	return unsolved;
}