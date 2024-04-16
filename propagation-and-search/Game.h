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
#include "ThreadSafeQueue.cpp"

class Game {
private:
	bool hasInput = false;
	std::string puzzle;

	Grid gameGrid;
	PossibleGrid possibleGrid; 
	bool hasSolution = true;

	bool depthFirstSearch();
public:
	Game(std::string path);
	bool evaluateBoard();
	void printResult() const;
};

#endif /* GAME_H_ */