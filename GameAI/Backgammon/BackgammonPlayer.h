//------------------------------------------------------------------------

#ifndef _BACKGAMMONPLAYER_H
#define _BACKGAMMONPLAYER_H

#include "BackgammonCommons.h"
#include "../Commons.h"
#include <SDL.h>
#include <vector>
#include "BackgammonMoveManager.h"
using namespace::std;

class Texture2D;

class BackgammonPlayer
{
//--------------------------------------------------------------------------------------------------
public:
	BackgammonPlayer(SDL_Renderer* renderer, BACKGAMMONCOLOUR colour, BackgammonBoard* board, SDL_Rect collisions[26]);
	~BackgammonPlayer();

	BACKGAMMONCOLOUR	 GetColour()			{return mTeamColour;}
	virtual void DealWithTurn(SDL_Event e);
//------------
public:
	virtual bool				DiceRoll(SDL_Event e);
	bool				bearOffReady = false;
	vector<BackgammonMove>		mSelectedAvailableMoves;

//--------------------------------------------------------------------------------------------------
protected:

	virtual bool DealWithMouseClick(SDL_Event e);
	void GetAllMoveOptions(BackgammonBoard board, BACKGAMMONCOLOUR mTeamColour, vector<BackgammonMove>* currentMoves);
	void MakeAMove(BackgammonBoard * board, BackgammonMove * move);

//--------------------------------------------------------------------------------------------------
protected:
	Texture2D*		  mSelectAPieceTexture;
	SDL_Rect		  mPawnPromotionDrawPosition;

	SDL_Rect boardCollisionBoxes[26];

	BackgammonBoard*			mBackgammonBoard;
	BACKGAMMONCOLOUR			mTeamColour;
	BACKGAMMONCOLOUR			mEnemyColour;
	vector<Vector2D>*			mHighlightPositions;		//Screen positioning, NOT board positioning.
	Vector2D*					mSelectedPiecePosition;

	vector<BackgammonMove>		mAllAvailableMoves;


	struct Die
	{
		Die() {};
		Die(Texture2D* tex, int num) { texture = tex; data = num; }
		Texture2D* texture;
		int data;
	};

	Die dice[6];
	Die* diceRolled[4];
	
	int pointSelected;
	int				  mNumberOfCheckersLeft;
	bool pieceSelected;

	string ColourAsString[3] = { "White", "None", "Red" };
};


#endif //_BACKGAMMONPLAYER_H