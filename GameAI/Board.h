#pragma once
#include <vector>

#include "Backgammon/BackgammonCommons.h"

struct Slot
{
	int pos =0; // relative to white home board
	BACKGAMMONCOLOUR occupiedTeam = NONE;
	int pieceCount = 0;

	SDL_Rect boundingBox;
};

struct UniqueSlot
{
	//White = -1; Red = 1;
	Slot teamSlots[2];

	int GetPieceCount(BACKGAMMONCOLOUR col) const
	{
		return teamSlots[col].pieceCount;
	}

	void AdjustPieceCount(const BACKGAMMONCOLOUR col, const int count)
	{
		teamSlots[col].pieceCount += count;
	}
};

//Require similar handling but apply differently
typedef UniqueSlot Bar;
typedef UniqueSlot BornOff;

const int boardslots = 24;
const int whiteStart = 23;
const int redStart = 0;

class Board
{

	BACKGAMMONCOLOUR currentLayout[totalPieceSlots][totalPiecesPerSlot];

	Slot slots[boardslots];
	Bar bar;
	BornOff bornOff;

	int redBornOff = 0;
	int whiteBornOff = 0;

	Board();

	Board(const Board& previous);

	void AcceptMove(BackgammonMove move);
	
	bool CanBearOff(BACKGAMMONCOLOUR col);

	BACKGAMMONCOLOUR GetOccupiedColour(int slot);
	int GetPieceCount(int slot);
	void SetUpNewBoardState();
	void LoadBoardState(char* path);
	vector<BackgammonMove> GenerateMovesFromSlot(vector<int> dice, BACKGAMMONCOLOUR col, int slot);
	int GetColStartSlot(BACKGAMMONCOLOUR col);
	int GetColEndSlot(BACKGAMMONCOLOUR col);
	vector<BackgammonMove> GenerateMoves(vector<int> dice, BACKGAMMONCOLOUR col, bool doubles);
};

