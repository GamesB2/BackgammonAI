//------------------------------------------------------------------------

#ifndef _BACKGAMMONPLAYERAI_H
#define _BACKGAMMONPLAYERAI_H

#include "BackgammonCommons.h"
#include "../Commons.h"
#include <SDL.h>
#include <vector>
#include "BackgammonMoveManager.h"
#include "BackgammonPlayer.h"
using namespace::std;

class Texture2D;

class BackgammonPlayerAI : public BackgammonPlayer
{
//--------------------------------------------------------------------------------------------------
public:
	BackgammonPlayerAI(SDL_Renderer* renderer, BACKGAMMONCOLOUR colour, BackgammonBoard* board, SDL_Rect collisions[25]);
	~BackgammonPlayerAI();

	BACKGAMMONCOLOUR	 GetColour()			{return mTeamColour;}
	void DealWithTurn(SDL_Event e);
	void Random(BackgammonBoard board);
	int ExpectiMiniMax(BackgammonBoard board, int depth);
	int Maximise(BackgammonBoard board, int depth);
	int Chance(BackgammonBoard board, int depth, MinMaxNext next);

	
//------------
public:
	bool				DiceRoll(SDL_Event e);
	void UseMovePairOnBoard(BackgammonMovePair pair, BackgammonBoard * board);
	void GetAllMovePairs(BackgammonBoard board, BACKGAMMONCOLOUR mTeamColour, vector<BackgammonMovePair> * vecPairs);
	bool				bearOffReady = false;

	void SetDebug() { debug = !debug; if (debug) { cout << "Debug Enabled" << endl; } else { cout << "Debug Disabled" << endl; } };

	void SetAI(int choice);

//--------------------------------------------------------------------------------------------------
protected:

	int Minimise(BackgammonBoard board, int deptH);

	int Star2Maximise(BackgammonBoard board, int depth, int alpha, int beta);
	int Star2Chance(BackgammonBoard board, int depth, int alpha, int beta, MinMaxNext next);
	int Star2Minimise(BackgammonBoard board, int depth, int alpha, int beta);

	int Probe(BackgammonBoard board, int depth,float alpha, float beta, int i, int j,  MinMaxNext next);

	int ScoreTheBoard(BackgammonBoard board);	

//--------------------------------------------------------------------------------------------------
protected:
	Texture2D*		  mSelectAPieceTexture;
	SDL_Rect		  mPawnPromotionDrawPosition;

	SDL_Rect* boardCollisionBoxes[26];

	BackgammonBoard*			mBackgammonBoard;
	BACKGAMMONCOLOUR			mTeamColour;
	BACKGAMMONCOLOUR			mEnemyColour;
	vector<Vector2D>*			mHighlightPositions;		//Screen positioning, NOT board positioning.
	Vector2D*					mSelectedPiecePosition;
	BackgammonMove*				mBestMove;
	vector<BackgammonMovePair>  mMovePairs;
	BackgammonMovePair			mBestMovePair;

	BACKGAMMONAISELECTION		mCurrentAI = STARMINIMAX;

	int pointSelected;
	int	mNumberOfCheckersLeft;

	bool debug = false;
};


#endif //_BACKGAMMONPLAYERAI_H