// Name        : Grid.h
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef GRID_H_
#define GRID_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iomanip> // setw
#include <iterator> // section
#include "Position.h"

class Grid {

private:
	int gridSize;
	int sectionSize;
	vector<vector<int>> grid;
	string puzzle;

public:
	Grid() : gridSize(0), sectionSize(0) {}
	explicit Grid(int size) : gridSize(size), sectionSize(int(sqrt(size))) {}

	void fill(const Position &pos, int value);

	// Conditions
	bool isLegal() const;
	bool isComplete() const;

	// File Operations
	bool read(const string &path);
	bool write() const;
	void printHorizontalLine() const;
	void print() const;

	// Retrieval
	const vector<int> getRow(int row) const;
	const vector<int> getCol(int col) const;
	const vector<int> getSection(int row, int col) const;
	const vector<Position> getUnsolvedPositions() const;
	const int getGridSize() { return gridSize; }
	const int getSectionSize() { return sectionSize; }
};

#endif /* GRID_H_ */
