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
	bool hasInput = false;
	std::string puzzlePath;
	Grid gameGrid;
	PossibleGrid possibleGrid;
	bool hasSolution = true;
	bool isDifficult = true;

	bool depthFirstSearch();
	int calculateFirstMovePossibilities();

public:
	Game(std::string path);
	bool evaluateBoard();
	void printResult() const;
};

#endif /* GAME_H_ */