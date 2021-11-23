#pragma once

#include <vector>
#include "Backgammon/BackgammonCommons.h"

struct PathwayNode
{
	MCTSNode* pBackgammonBoardNode;
	PathwayNode* pNext;
};

struct Pathway
{
public:
	int				Size	= 0;
	PathwayNode*	pHead	= nullptr;

	void PushFront(PathwayNode* pNode)
	{
		if (pHead == nullptr)
		{
			pHead = pNode;
			Size++;
		}
		else
		{
			pNode->pNext = pHead;
			pHead = pNode;
			Size++;
		}
	}
	PathwayNode* Get(const int& index)
	{
		int iteration = 0;
		return InternalGet(pHead, index, iteration);
	}

private:
	PathwayNode * InternalGet(PathwayNode* pNode, const int& index, int& iteration)
	{
		PathwayNode* pCurrent = pNode;
		if (index != iteration)
		{			
			iteration++;
			return InternalGet(pHead->pNext, index, iteration);
		}
		return pCurrent;
	}
};

int mTeamColour = 0;	// Forward Declaring stuff for compilation

bool Search(MCTSNode* current, std::vector<Pathway>& potentialWinPaths);
void GetAllMovePairs(BackgammonBoard board, BACKGAMMONCOLOUR mTeamColour, vector<BackgammonMovePair> * vecPairs);

void MonteCarloTreeSearch(BackgammonBoard* board)
{
	BackgammonBoard newBoard;

	MCTSNode rootNode = MCTSChanceNode();
	rootNode.board = *board;
	rootNode.playerTun = (BACKGAMMONCOLOUR)mTeamColour;
	
	std::vector<Pathway*> potentialWinPaths;

	Search(&rootNode, potentialWinPaths);

	// Do stuff with potential win paths
	// [..]
}

bool Search(MCTSNode* pCurrent, std::vector<Pathway*>& potentialWinPaths)
{
	std::vector<BackgammonMovePair> children;
	GetAllMovePairs(pCurrent->board, pCurrent->playerTun, &children);

	
}

void GetAllMovePairs(BackgammonBoard board, BACKGAMMONCOLOUR mTeamColour, vector<BackgammonMovePair>* vecPairs)
{

}