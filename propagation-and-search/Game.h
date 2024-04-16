// Name        : Game.h
// Author      : Hongbo Tian
// Editor      : Soleil Cordray

#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <functional>
#include "PossibleGrid.h"
#include "Grid.h"
#include "Position.h"

using namespace std;

class Game {
private:
	bool hasInput = false;
	string puzzle;

	Grid gameGrid;
	PossibleGrid possibleGrid; 
	bool hasSolution = true;

	bool depthFirstSearch();
public:
	Game(string path);
	bool evaluateBoard();
	void printResult() const;
};

#endif /* GAME_H_ */