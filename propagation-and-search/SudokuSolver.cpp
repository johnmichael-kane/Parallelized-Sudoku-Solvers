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
	cout << "THREADS: 1 to " << thread::hardware_concurrency() << "\n\n";
	cout << "Enter a file name (e.g., easy9.txt) and number of threads to start the game!\n\n";

	ifstream file;
	string filename;
	size_t numThreads;

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

	do {
		std::cout << "Number of threads: ";
		cin >> numThreads;

		bool validThread = numThreads >= 1 && numThreads <= thread::hardware_concurrency();
		if (!validThread) { 
			cout << "Invalid number of threads, try again!\n\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} else { break; }
	} while (true);

	// Run game

	auto start = chrono::high_resolution_clock::now();
	Game game(PATH + filename, numThreads); // initialize board
	while (!game.evaluateBoard()); // evaluate board
	auto end = chrono::high_resolution_clock::now();

	game.printResult();
	auto gameTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	cout << "\nTime Taken: " << gameTime << " milliseconds\n" << endl;
	return 0;
}

