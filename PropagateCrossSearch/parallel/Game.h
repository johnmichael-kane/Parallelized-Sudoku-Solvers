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
#include <atomic>
#include <future>
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
	size_t numThreads;

	bool depthFirstSearch();
	bool parallelDepthFirstSearch();
	int calculateFirstMovePossibilities();

public:
	Game(std::string path, size_t numThreads);
	bool evaluateBoard();
	void printResult() const;
};

#endif /* GAME_H_ */