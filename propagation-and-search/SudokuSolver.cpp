//========================================================================================
// Name        : SudokuSolver.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Start game, print solution & runtime
//========================================================================================

#include <iostream>
#include <fstream>
#include <chrono>
#include <cctype>
#include <string>
#include <algorithm>

#include "Game.h"

using namespace std::chrono;

int main()
{
	// Puzzle selection (user input).
	
	const string PATH = "puzzles/";
	std::cout << "\nLEVELS: easy, medium, hard\nSIZES: 9, 16, 25\n\n";
	std::cout << "Enter a file name (e.g., easy9.txt) to start the game!\n\n";
	ifstream file;
	string filename;

	do {
		std::cout << "File name: ";
		cin >> filename;
		file.open(PATH + filename);

		if (!file) (std::cout << "File not found, try again!\n\n");
	} while (!file);
	file.close();

	// Run game.

	auto start = high_resolution_clock::now();
	Game game(PATH + filename);
	while (!game.Evaluate());
	auto end = high_resolution_clock::now();

	game.printResult();

	auto gameTime = duration_cast<milliseconds>(end - start).count();
	cout << "\nTime Taken: " << gameTime << " milliseconds\n" << endl;
	return 0;
}

