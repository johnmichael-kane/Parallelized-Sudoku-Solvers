//========================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : ?
//========================================================================================

#include "Game.h"

Game::Game(string path) : puzzle(path) {
	hasSuccessInput = gameGrid.read(path);
	if (hasSuccessInput) {
		possibleGrid.setGrid(&gameGrid);
		possibleGrid.analyzeMoves(gameGrid);
	}
}

bool Game::evaluateBoard() {
	if (!hasSuccessInput || gameGrid.isComplete()) return gameGrid.isComplete();;

	possibleGrid.setGrid(&gameGrid);
	possibleGrid.analyzeMoves(gameGrid);
	bool trigger = true;

	// does parallelizing these loops allow for other work to be done?
	// is this really ideal since the point of the method is to start with linear constraints,
	// then with cross referencing, Then with DFS? i don't see a major benefit here.
	// does the order in which these are done really matter?

	// Linear Constraints: solve single-value positions.
	// parallelize loop
	// each thread has a copy of "trigger"
	#pragma omp parallel for reduction(&&: trigger) 
	for (size_t i = 0; i < possibleGrid.getUnsolvedPositions().size(); ++i)
	{
		const auto &pos = possibleGrid.getUnsolvedPositions()[i];
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1) { // unqique
			trigger = false;

			// mutual exclusion
			// a position's unsolved value is filled into the grid state if deemed unique
			// filled one thread at a time to ensure no unintended data modification
			lock_guard<mutex> lock(gridMutex);
			gameGrid.fill(pos, possibleValues.front());
		} // lock automatically released here (out of scope)
	}

	// Cross Reference: solve intersecting positions.
	if (trigger) {
		auto pairs = possibleGrid.crossReference();

		// parallelize loop
		#pragma omp parallel for 
		for (size_t i = 0; i < pairs.size(); ++i) {
			const auto &pair = pairs[i];

			// mutual exclusion
			// each pair (retrieved from cross referencing) filled to grid state one thread at a time
			// to ensure no unintended data modification
			lock_guard<mutex> lock(gridMutex);
			gameGrid.fill(pair.first, pair.second);
		}
		trigger = pairs.empty() ? trigger : false;
	}
	

	// Search: solve leftover positions across grid via complex searching.
	if (trigger && !gameGrid.isComplete()) {
		isFinished = depthFirstSearch();
	}

	return gameGrid.isComplete();
}





// Parallelized DFS
// multiple threads working on process-intensive grids at a time (& order doesn't matterO saves time
bool Game::depthFirstSearch() {
	// cout << "Depth First Search . . . " << endl;

	// initialize queue holding current state of game grid
	vector<Grid> searchQueue{gameGrid};
	// collect queue interaction results (grid states) from all threads
	vector<Grid> localSearchQueue;

	while (!searchQueue.empty()) {
		// parallelize searching of while loop
		#pragma omp parallel 
		{
			// private queue for each thread to avoid conflict & allow active integration
			vector<Grid> privateQueue;

			// distribute loop iterations amongst threads in parallel region, allowing for some 
			// threads to finish before others ("nowait") - which works with a queue's flexibility
			// QUESTION: how do distribute this work evenly? should it be?
			#pragma omp for nowait 
			for (size_t i = 0; i < searchQueue.size(); ++i) {
				// move current queue element into current grid
				Grid currentGrid = std::move(searchQueue[i]);

				// if current grid not completed or legal, queue element as a "possible value,a'
				// analyze its potential moves, and save the value & position of those possibilities.
				// then, for every position and corresponding valuem
				if (!currentGrid.isComplete() && currentGrid.isLegal()) {
					PossibleGrid possibilities;
					possibilities.setGrid(&currentGrid);
					possibilities.analyzeMoves(currentGrid);
					const auto &pos = possibilities.getUnsolvedPositions().front();
					const auto &posValues = possibilities.getPossibleValuesAt(pos.row, pos.col);

					// create copy of current grid state for each possible value, filling each grid
					// state with each possible value at their specified position (row, col).
					// move each potential grid state to the end of the searchQueue for later
					// processing
					for (int value : posValues) {
						Grid newGrid = currentGrid;
						newGrid.fill(pos, value);
						searchQueue.push_back(std::move(newGrid)); 
					}
				}

			}

			// combine solutions found by threads into localSearchQueue (this ensure updates are
			// controlled and Synchronized). only one thread can add to the localSearchQueue at a 
			// time (avoiding data conflicts while bringing together previously independent work)
			lock_guard<mutex> lock(gridMutex);
			localSearchQueue.insert(localSearchQueue.end(), 
									make_move_iterator(privateQueue.begin()), 
									make_move_iterator(privateQueue.end()));
		} // lock_guard released (out of scope)


		// localSearchQue becomes new searchQueue (integrate thread findings dynamically & without 
		// conflict, avoid unnecessary initializations)
		searchQueue = std::move(localSearchQueue);

		// QUESTION: what if there are multiple valid complete grid states? or is that not
		// possible with sudoku, or at least not relevant?

		// if search queue on last element (last possible grid), and the grid state of that element
		// is complete, a solution has been found - move the grid from the queue into the current game
		if (searchQueue.size() == 1 && searchQueue[0].isComplete()) {
			gameGrid = std::move(searchQueue[0]); 
			return true;
		}
	}

	// reached an empty queue, no solution found
	hasSolution = false;
	return false;
}

// Print resulting grid & whether a solution has been found.
void Game::printResult() const
{
	if (hasSuccessInput) {
		std::cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << std::endl;
		gameGrid.print();
	}
}