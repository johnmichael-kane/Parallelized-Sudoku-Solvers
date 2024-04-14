//========================================================================================
// Name        : Game.cpp
// Author      : Hongbo Tian
// Editor      : Soleil Cordray
// Description : ?
//========================================================================================

#include "Game.h"

Game::Game(string path) : puzzle(path),
						  isFinished(false),
						  hasSolution(true),
						  linearCycle(0),
						  crossReferenceCycle(0),
						  searchCycle(0)
{
	hasSuccessInput = gameGrid.read(path);
	possibleGrid.setGrid(&gameGrid);
	possibleGrid.analyzeMoves(gameGrid);
}

bool Game::Evaluate()
{
	if (hasSuccessInput)
	{
		possibleGrid.setGrid(&gameGrid);
		possibleGrid.analyzeMoves(gameGrid);
		bool methodTrigger = true;

		vector<Position>::const_iterator PosIt;
		for (PosIt = possibleGrid.getUnsolvedPositions().cbegin();
			 PosIt != possibleGrid.getUnsolvedPositions().cend(); ++PosIt)
		{
			if (possibleGrid.getPossibleValuesAt(PosIt->row, PosIt->col).size() == 1)
			{
				methodTrigger = false;
				gameGrid.fill(*PosIt,
							  possibleGrid.getPossibleValuesAt(PosIt->row, PosIt->col).at(0));
			}
		}

		if (methodTrigger)
		{
			vector<pair<Position, int>> pairs = possibleGrid.crossReference();
			vector<pair<Position, int>>::iterator it;
			for (it = pairs.begin(); it != pairs.end(); it++)
			{
				methodTrigger = false;
				gameGrid.fill(it->first, it->second);
			}
		}

		if (methodTrigger && !gameGrid.isComplete()) isFinished = search();
		return gameGrid.isComplete();
	}
	else
	{
		cout << "Invalid File Name" << endl;
		return false; 
	}
}

bool Game::search()
{
	cout << "Depth First Search . . . " << endl;
	bool success = true;

	vector<Grid> searchQueue;
	searchQueue.push_back(gameGrid);
	while (!searchQueue.operator[](searchQueue.size() - 1).isComplete() && searchQueue.size() > 0)
	{
		searchCycle++; 
		
		Grid currentGrid = searchQueue.at(searchQueue.size() - 1);

		if (currentGrid.isLegal())
		{
			PossibleGrid possGrid;
			possGrid.analyzeMoves(currentGrid);
			searchQueue.pop_back();
			Position pos = possGrid.getUnsolvedPositions().at(0);
			vector<int> posValues = possGrid.getPossibleValuesAt(pos.row, pos.col);

			for (vector<int>::iterator it = posValues.begin(); it != posValues.end(); it++)
			{
				Grid tempGrid = currentGrid;
				tempGrid.fill(pos, *it);
				searchQueue.push_back(tempGrid);
			}
		}
		else
		{
			searchQueue.pop_back();
		}
	}

	if (!searchQueue.empty()) 
	{
		success = true;
		gameGrid = searchQueue.back();
	}
	else
	{
		hasSolution = false;
	}
	return success; 
}

void Game::printResult() const
{
	stringstream ss;

	if (hasSolution)
	{
		ss << "Solved!!!";
	}
	else
	{
		ss << "Has no Solution.";
	}

	if (hasSuccessInput)
	{
		cout << ss.str() << endl;
		gameGrid.print();
	}
}