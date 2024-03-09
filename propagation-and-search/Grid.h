/*
 * Grid.h
 *
 *  Created on: 15 Jul 2015
 *      Author: Hongbo Tian
 *  Updated on: 8 Mar 2024
 * 		Editor: Soleil Cordray
 */

#ifndef GRID_H_
#define GRID_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Pos.h"

using namespace std;

class Grid
{

public:
	int dynamicSize;		   // Dynamic size of the grid, read from the file
	vector<vector<int>> mGrid; // Dynamic 2D vector to store the grid values

private:
	string mName; // Grid identifier

public:
	// Default constructor to initialize a Grid object.
	Grid();

	// Retrieves all values in a specified row.
	vector<int> getRow(int row);

	// Retrieves all values in a specified column.
	vector<int> getCol(int col);

	// Retrieves all values in a specific 3x3 section of the grid.
	// 'sRow' and 'sCol' specify the section by its top-left cell
	vector<int> getSection(int sRow, int sCol);

	// Retrieves all values in the 3x3 section containing a specific row and column.
	vector<int> getSectionByElement(int rRow, int rCol);

	// Returns a vector of Pos objects representing the positions of all unsolved cells.
	vector<Pos> getUnsolvedPos();

	// Fills a specified position 'pos' in the grid with the value 'value'.
	void fill(Pos pos, int value);

	// Checks if the current state of the grid adheres to Sudoku rules.
	bool isLegal();

	// Checks if the grid is completely filled in accordance with Sudoku rules.
	bool isComplete();

public:
	// Reads the grid state from a file specified by 'path'.
	bool read(string path);

	// Writes the current grid state to a file. The implementation and file path are not specified here.
	bool write();

	// Prints the current state of the grid to the console.
	void print();

private:
	// Helper function to get the starting index of a section based on a cell's row or column index.
	int getSectionStart(int num);
};
#endif /* GRID_H_ */
