#include "Board.h"

#include <vector>

Board::Board()
{
	for (int iter = 0; iter < boardslots; iter++)
	{
		slots[iter].pos = iter;
	}
	//if not loading state
	SetUpNewBoardState();
};

Board::Board(const Board& previous)
{
	for (int iter = 0; iter < boardslots; iter++)
	{
		slots[iter] = previous.slots[iter];
	}

	bar = previous.bar;
};

void Board::AcceptMove(BackgammonMove move)
{
	
}

bool Board::CanBearOff(BACKGAMMONCOLOUR col)
{
	if(bar.GetPieceCount(col) > 0)
	{
		return false;
	}

	int iterStart = GetColStartSlot(col);
	int iterEnd = GetColEndSlot(col);
	int diff = (int)col;

	//Kinda grim but haven't come up with a better solution yet
	if(col == WHITE)
	{
		for(int iter = 23; iter > 5; iter --)
		{
			if(slots[iter].occupiedTeam == col)
			{
				return false;
			}
		}
	}
	if(col == RED)
	{
		for (int iter = 5; iter < 23; iter++)
		{
			if (slots[iter].occupiedTeam == col)
			{
				return false;
			}
		}
	}

	return true;
}

BACKGAMMONCOLOUR Board::GetOccupiedColour(int slot)
{
	return slots[slot].occupiedTeam;
}

int Board::GetPieceCount(int slot)
{
	return slots[slot].pieceCount;
}

void Board::SetUpNewBoardState()
{

	slots[0].occupiedTeam = RED;
	slots[0].pieceCount = 2;

	slots[5].occupiedTeam = WHITE;
	slots[5].pieceCount = 5;

	slots[7].occupiedTeam = WHITE;
	slots[7].pieceCount = 3;

	slots[11].occupiedTeam = RED;
	slots[11].pieceCount = 5;

	slots[12].occupiedTeam = WHITE;
	slots[12].pieceCount = 5;

	slots[16].occupiedTeam = RED;
	slots[16].pieceCount = 3;

	slots[18].occupiedTeam = RED;
	slots[18].pieceCount = 5;

	slots[23].occupiedTeam = WHITE;
	slots[23].pieceCount = 2;
}

void Board::LoadBoardState(char* path)
{

}

vector<BackgammonMove> Board::GenerateMovesFromSlot(vector<int> dice, BACKGAMMONCOLOUR col, int slot)
{
	unsigned int iterCount = dice.size();

	vector<BackgammonMove> adjusted;
	if(bar.GetPieceCount(col) >0)
	{
		return adjusted;
	}


	//figure out a nice way to deal with this
	if (true) { iterCount = 1; }

	for(int i = 0; i < iterCount; i++)
	{
		
	}
}

int Board::GetColStartSlot(BACKGAMMONCOLOUR col)
{
	if(col == WHITE)
	{
		return whiteStart;
	}
	return redStart;
}

int Board::GetColEndSlot(BACKGAMMONCOLOUR col)
{
	if (col == WHITE)
	{
		return redStart;
	}
	return whiteStart;
}