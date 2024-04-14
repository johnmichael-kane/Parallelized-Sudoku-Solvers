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

using namespace std;
using namespace std::chrono;

int main()
{
	// Puzzle selection (user inputted filename)
	
	const string PATH = "puzzles/";

	cout << "\nGame Difficulties: easy, medium, hard" << endl;
	cout << "Board Sizes: 9 (9x9), 16 (16x16), 25 (25x25)" << endl;
	cout << "File Name Format: [mode][size].txt (e.g., easy9.txt)\n" << endl;

	ifstream file;
	string name;
	while (!file.is_open()) 
	{
		cout << "Enter a file name to start the game! >> " << flush;
		cin >> name;
		file.open(PATH + name);
		if (!file)
		{
			cout << "Invalid file name.\n" << endl;
			cin.clear();
		}
	}
	file.close();

	// Run game

	auto start_time = high_resolution_clock::now();

	Game game(PATH + name);
	while (!game.Evaluate())
	{
	}

	auto end_time = high_resolution_clock::now();

	game.printResult();

	// Measure game runtime

	auto game_time = duration_cast<milliseconds>(end_time - start_time).count();

	cout << "Time Taken: " << game_time << " milliseconds" << endl;

	return 0;
}

