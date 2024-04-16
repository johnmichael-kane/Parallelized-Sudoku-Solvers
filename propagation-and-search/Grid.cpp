//==================================================================================================
// Name        : Grid.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Implement Grid class by manipulating grid states, checking legality/completeness,
//				 and handling file-based input/output operations for Sudoku puzzles.
//==================================================================================================

#include "Grid.h"

using namespace std;

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
bool Grid::isLegal() const {
	auto isUnique = [](const std::vector<int> &vec) -> bool {
		vector<int> filteredVector;
		copy_if(vec.begin(), vec.end(), back_inserter(filteredVector), [](int v)
					 { return v != 0; }); // remove zeroes
		sort(filteredVector.begin(), filteredVector.end());
		auto last = unique(filteredVector.begin(), filteredVector.end());
		return last == filteredVector.end(); // unique: nothing removed
	};

	// CHECK THESE CONCURRENTLY?
	for (int i = 0; i < gridSize; ++i) {
		if (!isUnique(getRow(i)) || !isUnique(getCol(i)) 
			|| !isUnique(getSection(i / sectionSize, i % sectionSize))) // ith row, ith col
			return false;
	}

	return true;
}

// Verify that no rows contain empty cells (if empty cells exist, return false).
bool Grid::isComplete() const {
	return isLegal() && std::none_of(grid.begin(), grid.end(), [](const std::vector<int> &row)
						{ return any_of(row.begin(), row.end(), [](int cell)
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

// Print helper (horizontal line).
void Grid::printHorizontalLine() const {
	int calculation = 0;
	calculation = (gridSize == 9) ? (3 * sectionSize) : ((3 * sectionSize) + 1);

	string section = "*" + string(calculation, '-');
	string horizontal = string(gridSize / sectionSize, ' ').replace(0, section.size(), section);

	for (int i = 0; i < sectionSize; ++i) { cout << horizontal; }

	std::cout << "*\n";
};

// Print grid to console.
void Grid::print() const
{
	printHorizontalLine(); // top
	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			// beginning row & after section
			if ((j + sectionSize) % sectionSize == 0) cout << '|';
			
			// numbers
			if (gridSize == 9) {
				cout << setw(2) << grid[i][j] << ' ';
			} else {
				if (j % sectionSize == 0) std::cout << ' ';
				cout << setw(2) << grid[i][j] << ' ';
			}
		}
		cout << "|\n"; // end row
		if ((i + 1) % sectionSize == 0 && (i + 1) < gridSize) printHorizontalLine(); // row sect
	}
	printHorizontalLine(); // bottom
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
	int startRow = (row / sectionSize) * sectionSize;
	int startCol = (col / sectionSize) * sectionSize;
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