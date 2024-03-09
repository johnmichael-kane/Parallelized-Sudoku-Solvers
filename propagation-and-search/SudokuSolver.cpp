//============================================================================
// Name        : Sudoku.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Main Function
//============================================================================

#include <iostream>
#include <chrono>

#include "Game.h"

using namespace std;
using namespace std::chrono;

int main()
{
	const string PATH = "puzzles/";

	// Taking user inputs
	string name;
	cout << "Enter File Name >> " << flush;
	cin >> name;

	// Timer start
	auto tStart = high_resolution_clock::now();

	// Initialise Game & Game Loop
	Game game(PATH + name);
	while (!game.Evaluate())
	{
	}

	// Timer end
	auto tEnd = high_resolution_clock::now();

	// Print solution and time
	game.printStats();

	// Calculate and convert to milliseconds
	auto duration = duration_cast<milliseconds>(tEnd - tStart).count();

	cout << "Time Taken: " << duration << " milliseconds" << endl;

	return 0;
}
