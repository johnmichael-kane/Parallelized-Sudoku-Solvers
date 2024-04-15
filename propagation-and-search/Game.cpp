//========================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : ?
//========================================================================================

#include "Game.h"

// grid exists if file successfully read. 
// if file successfully read, initialize the grid & analyze it for potential moves
Game::Game(string path) : puzzle(path) {
	gridExists = gameGrid.read(path);
	if (gridExists) {
		possibleGrid.setGrid(&gameGrid);
		possibleGrid.analyzeMoves(gameGrid);
	}
}

void processSegment(int start, int end, bool &localTrigger)
{
	for (int i = start; i < end; ++i)
	{
		const auto &pos = possibleGrid.getUnsolvedPositions()[i];
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1)
		{
			localTrigger = false;
			std::lock_guard<std::mutex> lock(gridMutex);
			gameGrid.fill(pos, possibleValues.front());
		}
	}
}

// while an unfinished grid exists,
// 1) set the grid
// 
bool Game::evaluateBoard() {
	if (!gridExists || gameGrid.isComplete()) return gameGrid.isComplete();; // bool

	bool trigger = true;

	#pragma omp parallel for reduction(&&: trigger) 
	for (size_t i = 0; i < possibleGrid.getUnsolvedPositions().size(); ++i)
	{
		const auto &pos = possibleGrid.getUnsolvedPositions()[i];
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1) { // unique
			trigger = false;

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

			#pragma omp for nowait 
			for (size_t i = 0; i < searchQueue.size(); ++i) {
				// move current queue element into current grid
				Grid currentGrid = std::move(searchQueue[i]);

				if (!currentGrid.isComplete() && currentGrid.isLegal()) {
					PossibleGrid possibilities;
					possibilities.setGrid(&currentGrid);
					possibilities.analyzeMoves(currentGrid);
					const auto &pos = possibilities.getUnsolvedPositions().front();
					const auto &posValues = possibilities.getPossibleValuesAt(pos.row, pos.col);

					for (int value : posValues) {
						Grid newGrid = currentGrid;
						newGrid.fill(pos, value);
						searchQueue.push_back(std::move(newGrid)); 
					}
				}

			}
			lock_guard<mutex> lock(gridMutex);
			localSearchQueue.insert(localSearchQueue.end(), 
									make_move_iterator(privateQueue.begin()), 
									make_move_iterator(privateQueue.end()));
		} // lock_guard released (out of scope)

		searchQueue = std::move(localSearchQueue);

		if (searchQueue.size() == 1 && searchQueue[0].isComplete()) {
			gameGrid = std::move(searchQueue[0]); 
			return true;
		}
	}

	// reached an empty queue, no solution found
	hasSolution = false;
	return false;
}

// TRIGGER MEANING?

// loop through specified segment:
// get spot
// get possibilities at spot
// if only 1 possibility,
// fill spot in game grid
void processSegment(int start, int end) comst
{
	bool localTrigger = true;
	for (int i = start; i < end; ++i) {
		const auto &pos = possibleGrid.getUnsolvedPositions()[i];
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);

		if (possibleValues.size() == 1) {
			localTrigger = false;

			// QUESTION: HOW TO PREVENT THREADS WAITING ON EACH OTHER?
			// NOTE: REFER TO LAST CODE NOTES

			// lock before other threads can interfere
			lock_guard<mutex> lock(gridMutex);
			gameGrid.fill(pos, possibleValues.front());
		} 
	}
	std::lock_guard<std::mutex> lock(gridMutex);
	trigger = trigger && localTrigger; 
}

void Game::printResult() const
{
	if (gridExists) {
		std::cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << std::endl;
		gameGrid.print();
	}
}