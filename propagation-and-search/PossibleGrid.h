//========================================================================================
// Name        : PossibleGrid.h
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
//========================================================================================

#ifndef POSSIBLEGRID_H_ 
#define POSSIBLEGRID_H_

#include <vector>
#include <set>
#include "Grid.h" 
#include "Pos.h"  

class PossibleGrid
{
public:
	Grid *grid;
	vector<vector<vector<int>>> possibleGrid;
	vector<Pos> unsolvedPositions;

	PossibleGrid() : grid(nullptr) {}

	void setGrid(Grid *g)
	{
		grid = g;
	}

	void clear();
	vector<int> invalidNumbers(const Post &pos) const;
	void Analysis();
	std::vector<std::pair<Pos, int>> crossReference();
	std::string vectorsToString(std::vector<int> &vector);
	void print();
};
#endif /* POSSIBLEGRID_H_ */
