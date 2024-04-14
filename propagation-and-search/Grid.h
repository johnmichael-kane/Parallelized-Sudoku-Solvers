//========================================================================================
// Name        : Grid.h
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
//========================================================================================

#ifndef GRID_H_
#define GRID_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>
#include "Pos.h"

using namespace std;

class Grid {

public:
	int gridSize;
	int sectionSize;
	vector<vector<int>> gridValues;

private:
	string gridName;

public:
	Grid() : gridSize(0), sectionSize(0), gridName("") {}

	void fill(const Pos &pos, int value);
	bool isLegal() const;
	bool isComplete() const;
	bool isUnique(const std::vector<int> &values) const;

	// Getters
	vector<int> getRow(int row) const;
	vector<int> getCol(int col) const;
	vector<int> getSection(int row, int col) const;
	vector<Pos> getUnsolvedPositions() const;

	// File
	bool read(const string &path);
	bool write() const;
	void print() const;
};

#endif /* GRID_H_ */
