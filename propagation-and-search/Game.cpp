//==================================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : Implements methods to solve a Sudoku board using linear constraint propagation,
//               cross-referencing, and depth-first search (DFS), potentially in parallel based on
//               puzzle difficulty. Returns true if the board is solved.
//==================================================================================================

#include "Game.h"
#include <vector>
#include <future>
using namespace std;

Game::Game(string path, size_t threads) : puzzlePath(path), numThreads(threads) {
	hasInput = gameGrid.read(path);
	isDifficult = (path.find("hard") != string::npos); // Check if the puzzle is labeled as 'hard'
	possibleGrid.setGrid(&gameGrid, threads);		   // Initialize grid with possible values
}

// Evaluates the board and applies appropriate solving methods.
bool Game::evaluateBoard() {
	if (!hasInput || gameGrid.isComplete()) return gameGrid.isComplete();

	possibleGrid.analyzeMoves(gameGrid); // Calculate possible moves
	bool cellsLeft = true;

	// Linear Constraint Propagation: Solve cells with a single possible value
	auto unsolvedCells = possibleGrid.getUnsolvedPositions();
	for (const auto &pos : unsolvedCells) {
		const auto &possibleValues = possibleGrid.getPossibleValuesAt(pos.row, pos.col);
		if (possibleValues.size() == 1) {
			gameGrid.fill(pos, possibleValues.front()); // Fill in the single possible value
			cellsLeft = false;
		}
	}

	// Cross Referencing: Solve cells by checking intersecting rows, columns, and boxes
	if (cellsLeft) {
		auto pairs = possibleGrid.crossReference();
		for (const auto &pair : pairs) {
			cellsLeft = false;
			gameGrid.fill(pair.first, pair.second); // Fill intersecting cells
		}
	}

	// Depth First Search: Apply DFS if there are still unsolved cells
	if (cellsLeft && !gameGrid.isComplete()) {
		hasSolution = isDifficult ? parallelDepthFirstSearch() : depthFirstSearch();
	}

	return gameGrid.isComplete(); // Return true if the board is completely solved
}

// Calculates the number of possibilities for the most constrained cell.
int Game::calculateFirstMovePossibilities() {
	auto unsolvedPositions = possibleGrid.getUnsolvedPositions();
	if (unsolvedPositions.empty()) return 0;

	auto minPosIt = min_element(unsolvedPositions.begin(), unsolvedPositions.end(),
								[this](const Position &a, const Position &b)
								{
									return possibleGrid.getPossibleValuesAt(a.row, a.col).size() <
										   possibleGrid.getPossibleValuesAt(b.row, b.col).size();
								});

	return possibleGrid.getPossibleValuesAt(minPosIt->row, minPosIt->col).size();
}

// Executes a parallel DFS if the puzzle is complex.
bool Game::parallelDepthFirstSearch() {
	int initial_branches = min(numThreads, static_cast<size_t>(calculateFirstMovePossibilities()));
	initial_branches = max(1, initial_branches);

	vector<future<bool>> futures;
	for (int i = 0; i < initial_branches; ++i) {
		futures.push_back(async(launch::async, [this]
								{ return this->depthFirstSearch(); }));
	}

	for (auto &fut : futures) {
		if (fut.get())
			return true; // Return true if any thread finds a solution
	}

	return false; // Return false if no solutions were found
}

// Depth-first search implementation for solving the Sudoku puzzle.
bool Game::depthFirstSearch()
{
	vector<Grid> searchStack = {gameGrid}; // Use a vector as a stack for DFS

	while (!searchStack.empty()) {
		Grid currentGrid = std::move(searchStack.back());
		searchStack.pop_back();

		if (currentGrid.isComplete()) {
			gameGrid = std::move(currentGrid);
			return true; // Puzzle solved
		}

		if (!currentGrid.isLegal()) continue; // Skip illegal grid configurations

		PossibleGrid possibilities;
		possibilities.setGrid(&currentGrid, numThreads);
		possibilities.analyzeMoves(currentGrid);

		auto unsolvedPositions = possibilities.getUnsolvedPositions();
		if (unsolvedPositions.empty())
			continue;

		auto pos = *min_element(unsolvedPositions.begin(), unsolvedPositions.end(),
								[&possibilities](const Position &a, const Position &b)
								{
									return possibilities.getPossibleValuesAt(a.row, a.col).size() <
										   possibilities.getPossibleValuesAt(b.row, b.col).size();
								});

		for (int value : possibilities.getPossibleValuesAt(pos.row, pos.col)) {
			Grid newGrid = currentGrid;
			newGrid.fill(pos, value);
			searchStack.push_back(std::move(newGrid)); // Push new state to stack
		}
	}

	return false; // No solution found
}

// Prints the result of the Sudoku solver.
void Game::printResult() const {
	if (hasInput) {
		cout << (hasSolution ? "\nSolved!!!" : "\nNo Solution.") << endl;
		gameGrid.print();
	}
}
