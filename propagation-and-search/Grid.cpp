/*
 * Grid.cpp
 *
 *  Created on: 15 Jul 2015
 *      Author: Hongbo Tian
 *  Updated on: 8 Mar 2024
 * 		Editor: Soleil Cordray
 */

#include "Grid.h"

// Constructor initializing the grid with an empty name.
Grid::Grid() : mName(""), dynamicSize(0)
{
}

// Fills a cell at the specified position with a value.
void Grid::fill(Pos pos, int value)
{
	mGrid[pos.row][pos.col] = value;
}

// Checks if the grid does not violate Sudoku rules.
// by ensuring there are no repeated values in any row or column.
bool Grid::isLegal()
{
	bool legal = true;

	// Check each row and column for duplicates. (check non-repeats)
	for (int i = 0; i < dynamicSize; i++)
	{
		vector<int> vRow = getRow(i); // Retrieve all values in the current row.
		vector<int> vCol = getCol(i); // Retrieve all values in the current column.

		// The unique algorithm rearranges the elements in the range [first, last) in such a way that each element
		// that is equal to the preceding element appears only once. The relative order of elements that are not duplicates
		// is preserved, and the physical size of the container is not altered.
		// Applying 'unique' does not by itself remove duplicates because the end of the non-duplicate range
		// is returned but elements are not erased.
		vector<int>::iterator uniqueRow = unique(vRow.begin(), vRow.end());
		vector<int>::iterator uniqueCol = unique(vCol.begin(), vCol.end());

		// Check if there are elements beyond the 'unique' range by comparing the iterator to the end of the vector.
		// If the 'unique' iterator does not equal the end iterator, it indicates there were duplicate elements
		// in the row or column, marking the grid as potentially illegal.
		if (uniqueRow != vRow.end())
		{
			legal = false; // Found duplicates in a row, so the grid is not legal.
		}
		if (uniqueCol != vCol.end())
		{
			legal = false; // Found duplicates in a column, so the grid is not legal.
		}
	}

	// Additionally, checks each 3x3 section for non-repeating values, similar to the row and column checks.
	for (int sRow = 0; sRow < 3; sRow++)
	{
		for (int sCol = 0; sCol < 3; sCol++)
		{
			// Subsections 
			vector<int> vSection = getSection(sRow, sCol); // Retrieve values in the current 3x3 section.
			vector<int>::iterator uniqueSection = unique(vSection.begin(), vSection.end());
			if (uniqueSection != vSection.end())
			{
				legal = false; // Found duplicates in a section, so the grid is not legal.
			}
		}
	}

	return legal; // Return true if no duplicates were found in rows, columns, and sections; false otherwise.
}

// Determines if the grid is completely and correctly filled out.
bool Grid::isComplete()
{
	bool complete = isLegal();

	// Create a vector representing the expected numbers in each row, column, or section.
	vector<int> identity;
	for (int i = 0; i < dynamicSize; i++)
	{
		identity.push_back(i + 1);
	}

	// Check each row and column against the identity vector.
	for (int i = 0; i < dynamicSize; i++)
	{
		vector<int> vRow = getRow(i);
		vector<int> vCol = getCol(i);

		// Sort before comparison to ensure the order matches the identity vector.
		sort(vRow.begin(), vRow.end());
		sort(vCol.begin(), vCol.end());

		if (vRow != identity || vCol != identity)
		{
			complete = false;
		}
	}

	// Check each 3x3 section against the identity vector. **** SIZE ****
	for (int sRow = 0; sRow < 3; sRow++)
	{
		for (int sCol = 0; sCol < 3; sCol++)
		{
			vector<int> vSection = getSection(sRow, sCol);
			sort(vSection.begin(), vSection.end());
			if (identity != vSection)
			{
				complete = false;
			}
		}
	}

	return complete;
}

// Getters for row, column, and sections

vector<int> Grid::getRow(int row) {
	vector<int> vRow;
	for (int col = 0; col < dynamicSize; col++)
	{
		if (mGrid[row][col] != 0) {
			vRow.push_back(mGrid[row][col]);
		}
	}
	return vRow;
}

vector<int> Grid::getCol(int col) {
	vector<int> vCol;
	for (int row = 0; row < dynamicSize; row++)
	{
		if (mGrid[row][col] != 0) {
			vCol.push_back(mGrid[row][col]);
		}
	}
	return vCol;
}

vector<int> Grid::getSection(int sRow, int sCol) {
	vector<int> vSection;
	for (int iRow = 0; iRow < 3; iRow++) {
		for (int iCol = 0; iCol < 3; iCol++) {
			int row = sRow * 3 + iRow;
			int col = sCol * 3 + iCol;
			if (mGrid[row][col] != 0) {
				vSection.push_back(mGrid[row][col]);
			}
		}
	}
	return vSection;
}

vector<int> Grid::getSectionByElement(int rRow, int rCol) {

	int sRow = getSectionStart(rRow);
	int sCol = getSectionStart(rCol);

	return getSection(sRow, sCol);
}

vector<Pos> Grid::getUnsolvedPos() {
	vector<Pos> unslovedPos;
	for (int row = 0; row < dynamicSize; row++)
	{
		for (int col = 0; col < dynamicSize; col++)
		{
			if (mGrid[row][col] == 0) {
				unslovedPos.push_back(Pos(row, col));
			}
		}
	}
	return unslovedPos;
}

// Read and write to files 
// **** CHANGES: ****
	// give the user a list of available files & their difficulty
	// have them type out the filename they want to test (test for correctness)
bool Grid::read(string path)
{
	ifstream myFile(path);
	if (!myFile.is_open())
	{
		return false;
	}

	// First line now determines the size of the grid.
	myFile >> dynamicSize;
	myFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip to the next line

	mGrid.resize(dynamicSize, vector<int>(dynamicSize));

	for (int row = 0; row < dynamicSize; ++row)
	{
		for (int col = 0; col < dynamicSize; ++col)
		{
			myFile >> mGrid[row][col];
		}
	}

	myFile.close();
	return true;
}

// Writes the current state of the Sudoku grid to a file.
bool Grid::write()
{
	bool success = true; // Initially, assume the write operation will be successful.

	// Prepare the file name by prefixing "ANS" to the original file name (stored in mName).
	// This indicates that this file is an answer or solution file.
	string name = "ANS" + mName;

	// Create an output file stream to write to the file named "ANS"+mName.
	ofstream file(name.c_str());

	if (file.is_open()) // Check if the file was successfully opened for writing.
	{
		// Iterate over each row of the grid.
		for (int i = 0; i < dynamicSize; i++)
		{
			// Iterate over each column in the current row.
			for (int j = 0; j < dynamicSize; j++)
			{
				// Write the value at the current cell to the file.
				file << mGrid[i][j];
				// Follow each number with a space for readability and formatting.
				file << " ";
			}
			// After writing all columns of the current row, write a newline character to move to the next row in the file.
			file << "\n";
		}
		// After writing the entire grid to the file, close the file stream.
		file.close();
	}
	else
	{
		// If the file could not be opened for writing, set success to false to indicate failure.
		success = false;
	}

	// Return the success status of the write operation.
	return success;
}

void Grid::print() {
	for (int row = 0; row < dynamicSize; row++)
	{
		for (int col = 0; col < dynamicSize; col++)
		{
			cout << mGrid[row][col] << " " << flush;
		}
		cout << endl;
	}
}

// Misc
int Grid::getSectionStart(int num) 
{
	int sectionSize = sqrt(dynamicSize);
	int start;

	switch (dynamicSize)
	{
		case 9:
			if (num < 3)
			{
				start = 0;
			}
			else if (num < 6)
			{
				start = 1;
			}
			else
			{
				start = 2;
			}
			break;

		case 16:
			if (num < 4)
			{
				start = 0;
			}
			else if (num < 8)
			{
				start = 1;
			}
			else if (num < 12)
			{
				start = 2;
			}
			else if (num < 16)
			{
				start = 3;
			}
			else
			{
				start = 4;
			}
			break;

		case 25:
			if (num < 5)
			{
				start = 0;
			}
			else if (num < 10)
			{
				start = 1;
			}
			else if (num < 15)
			{
				start = 2;
			}
			else if (num < 20)
			{
				start = 3;
			}
			else
			{
				start = 4;
			}
			break;
		default:
			std::cout << "Could not retrieve start of section." << std::endl;
			break;
	}

	return start;
}

/*
int Grid::getSectionStart(int num)
{
	int sectionSize = sqrt(dynamicSize); // Assuming dynamicSize is the total grid size, this needs to be adjusted
	int start = (num / sectionSize) * sectionSize;
	return start;
}
*/