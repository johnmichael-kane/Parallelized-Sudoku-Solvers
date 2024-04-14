//========================================================================================
// Name        : Game.h
// Author      : Hongbo Tian (Created 15 Jul 2015)
// Editor      : Soleil Cordray (Updated 8 Mar 2024)
//========================================================================================

// Game.h: Header file for the Game class, which encapsulates the logic and state for a puzzle game.
// This class is responsible for managing the game state, evaluating game progress, and printing game statistics.

#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <vector>
#include <string>
#include "PossibleGrid.h"
#include "Grid.h"
#include "Pos.h"

class Game {
private:
	string gameFile;
	bool isFinished;
	bool hasSolution;
	Grid gameGrid;
	PossibleGrid possibleGrid;

public:
	Game(const string &path);
	bool Evaluate();
	void printStats() const;

private:
	bool linearConstraintSolution();
	bool crossReferenceSolution();
	bool depthFirstSearch();
};

#endif /* GAME_H_ */ 
