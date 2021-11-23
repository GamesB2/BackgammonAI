#pragma once
#include "BackgammonCommons.h"
#include "BackgammonConstants.h"
#include <SDL.h>

class BackgammonMoveManager
{
public:
	~BackgammonMoveManager();

	static BackgammonMoveManager* Instance();

	void StoreAvailableMoves(int pointFrom, BackgammonBoard * board, BACKGAMMONCOLOUR player, BACKGAMMONCOLOUR opponent);
	void StoreMovesFromBar(BackgammonBoard * board, BACKGAMMONCOLOUR player, BACKGAMMONCOLOUR opponent);
	void StoreBearOffMoves(BackgammonBoard * board, int Pointfrom, BACKGAMMONCOLOUR player, BACKGAMMONCOLOUR opponent);
	bool AttemptToMakeMove(SDL_Event e, BackgammonBoard * mBackgammonBoard, BACKGAMMONCOLOUR mTeamColour, BACKGAMMONCOLOUR mEnemyColour);
	bool AttemptBearOff(SDL_Event e, BackgammonBoard * mBackgammonBoard, BACKGAMMONCOLOUR mTeamColour, BACKGAMMONCOLOUR mEnemyColour);
	bool AttemptToMakeMoveFromBar(SDL_Event e, BackgammonBoard * mBackgammonBoard, BACKGAMMONCOLOUR mTeamColour, BACKGAMMONCOLOUR mEnemyColour);
	void PrepareForNextMove(BackgammonBoard * board, BACKGAMMONCOLOUR player, BACKGAMMONCOLOUR opponent);

	//--------------------------------------------------------------------------------------------------
private:
	BackgammonMoveManager();

	//--------------------------------------------------------------------------------------------------
private:
	static BackgammonMoveManager* mInstance;
};

