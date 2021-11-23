#pragma once
#ifndef _GAMESCREEN_BACKGAMMON_H
#define _GAMESCREEN_BACKGAMMON_H

#include "../GameScreen.h"
#include "../Commons.h"
#include "BackgammonCommons.h"
#include "BackgammonConstants.h"
#include "BackgammonPlayer.h"
#include "BackgammonPlayerAI.h"
#include <SDL.h>
#include <vector>

using namespace::std;

class Texture2D;

class GameScreen_Backgammon : GameScreen
{
public:
	GameScreen_Backgammon(SDL_Renderer* renderer);
	~GameScreen_Backgammon();

	void Render();
	void Update(size_t deltaTime, SDL_Event e);
	void SetDice();

private:
	void RenderPieces();
	
	Texture2D*		 mBackgroundTexture;
	Texture2D*		 mWhiteChecker;
	Texture2D*		 mRedChecker;
	Texture2D*		 mSelectedPieceHighlight;
	Texture2D*		 mAvailableMoveHighlight;
	Texture2D*		 mDiceOne;
	Texture2D*		 mDiceTwo;
	Texture2D*		 mDiceThree;
	Texture2D*		 mDiceFour;
	Texture2D*		 mDiceFive;
	Texture2D*		 mDiceSix;
	Texture2D*		 mDiceBlank;
	Texture2D*		 mRollButton;
	Texture2D*		 mNumberSpriteSheet;
	Texture2D*		 mBearOffHighlight;

	Vector2D		 mButtonPosition;

	bool movesLeft;

	int mPlayerTurn;

	int piecesOnBar;

	BACKGAMMONGAMESTATE mCurrentGameState;

	BackgammonBoard* mBackgammonBoard;
	SDL_Rect boardCollisionBoxes[26];

	BackgammonPlayer*	 mPlayers[2];
	//COLOUR			 mPlayerTurn;
	bool			 mAIPlayerPlaying;

	Vector2D		 mSelectedPiecePosition;
	vector<Vector2D> mHighlightPositions;
	bool			 mHighlightsOn;

	SDL_Renderer	*mRenderer;
	size_t			 mPreTurnTextTimer;
	int*			 mSearchDepth;

};

#endif //_GAMESCREEN_BACKGAMMON_H