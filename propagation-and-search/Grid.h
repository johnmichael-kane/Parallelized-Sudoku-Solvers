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
#include "Position.h"

class Grid {

public:
	int gridSize;
	int sectionSize;
	vector<vector<int>> grid;
	string puzzle;

	Grid() : gridSize(0), sectionSize(0) {}
	explicit Grid(int size) : gridSize(size), sectionSize(int(sqrt(size))) {}

	void fill(const Position &pos, int value);

	// Conditions
	bool isLegal();
	bool isComplete();

	// File Operations
	bool read(const string &path);
	bool write() const;
	void print() const;
	
	// Retrieval
	const vector<int> getRow(int row) const;
	const vector<int> getCol(int col) const;
	const vector<int> getSection(int row, int col) const;
	const vector<Position> getUnsolvedPositions() const;

	int getSectionStart(int num) const { return (num / sectionSize) * sectionSize; }
};

#endif /* GRID_H_ */
