//========================================================================================
// Name        : SudokuSolver.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Start game, print solution & runtime
//========================================================================================

#include <iostream>
#include <fstream>
#include <chrono>
#include <limits>

#include "Game.h"

using namespace std;

int main() {
	// Puzzle selection (user input)

	const string PATH = "puzzles/";
	cout << "\nLEVELS: easy, medium, hard\nSIZES: 9, 16, 25\n";
	cout << "Enter a file name (e.g., easy9.txt) to start the game!\n\n";

	ifstream file;
	string filename;

	do {
		std::cout << "File name: ";
		cin >> filename;
		file.open(PATH + filename);
		if (!file) { 
			cout << "File not found, try again!\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while (!file);
	file.close();

	// Run game

	auto start = chrono::high_resolution_clock::now();
	Game game(PATH + filename); // initialize board
	while (!game.evaluateBoard()); // evaluate board
	auto end = chrono::high_resolution_clock::now();

	game.printResult();
	auto gameTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	cout << "\nTime Taken: " << gameTime << " milliseconds\n" << endl;
	return 0;
}

