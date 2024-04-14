// Name        : Game.h
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <vector>
#include "PossibleGrid.h"
#include "Grid.h"
#include "Position.h"

class Game {
private:
	string puzzle;
	bool isFinished = false;
	bool hasSolution = true;
	bool hasSuccessInput = false;
	Grid gameGrid;
	PossibleGrid possibleGrid;
	int linearCycle = 0;
	int crossReferenceCycle = 0;
	int searchCycle = 0;

	bool search();
public:
	Game(string path);
	bool Evaluate();
	void printResult() const;
};

#endif /* GAME_H_ */