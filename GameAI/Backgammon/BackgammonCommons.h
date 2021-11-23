#ifndef BACKGAMMONCOMMONS_H
#define BACKGAMMONCOMMONS_H

#include "BackgammonConstants.h"
#include "../Commons.h"
#include <SDL.h>

static int boardsCreated;

enum BACKGAMMONAISELECTION
{
	EXPECTIMAX,
	STARMINIMAX,
	RANDOM
};

enum BACKGAMMONCOLOUR
{
	NONE = 0,
	WHITE = -1,
	RED =1,	
};

struct Piece
{
	Vector2D Position;
	SDL_Rect drawPos;
	BACKGAMMONCOLOUR colour = NONE;
};

const int totalPieceSlots = 25;
const int totalPiecesPerSlot = 15;

struct BackgammonBoard
{
	BACKGAMMONCOLOUR currentLayout[totalPieceSlots][totalPiecesPerSlot];

	Piece boardPositions[totalPieceSlots][totalPiecesPerSlot];
	Piece* selected;
	
	int redPiecesOnBar = 0;
	int whitePiecesOnBar = 0;

	int pieceSelectedArrayPos[2];
	bool pieceSelected = false;

	bool redBearOff = false;
	bool whiteBearOff = false;
	int redPiecesOffBoard = 0;
	int whitePiecesOffBoard = 0;

	int diceAvailable[4];
	int movesAvailable;

	BackgammonBoard()
	{
		ZeroMemory(this, sizeof(BackgammonBoard));
		SetUpBlankBoard();
	};

	int GetTeamBarCount(BACKGAMMONCOLOUR col)
	{
		if(col == RED)
		{
			return redPiecesOnBar;
		}
		else
		{
			return whitePiecesOnBar;
		}
	}

	bool GetTeamBearOff(BACKGAMMONCOLOUR col)
	{
		if(col == RED)
		{
			return redBearOff;
		}
		else
		{
			return whiteBearOff;
		}
	}
	
	BackgammonBoard(BACKGAMMONCOLOUR previous[25][15])
	{
		for (int x = 0; x < 25; x++)
		{
			for (int y = 0; y < 15; y++)
			{
				currentLayout[x][y] = previous[x][y];
			}
		}
	};
	
	void SetUpBlankBoard()
	{
		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				currentLayout[i][j] = NONE;
			}
		}
		currentLayout[1][0] = RED;
		currentLayout[1][1] = RED;

		currentLayout[6][0] = WHITE;
		currentLayout[6][1] = WHITE;
		currentLayout[6][2] = WHITE;
		currentLayout[6][3] = WHITE;
		currentLayout[6][4] = WHITE;

		currentLayout[8][0] = WHITE;
		currentLayout[8][1] = WHITE;
		currentLayout[8][2] = WHITE;

		currentLayout[12][0] = RED;
		currentLayout[12][1] = RED;
		currentLayout[12][2] = RED;
		currentLayout[12][3] = RED;
		currentLayout[12][4] = RED;

		currentLayout[13][0] = WHITE;
		currentLayout[13][1] = WHITE;
		currentLayout[13][2] = WHITE;
		currentLayout[13][3] = WHITE;
		currentLayout[13][4] = WHITE;

		currentLayout[17][0] = RED;
		currentLayout[17][1] = RED;
		currentLayout[17][2] = RED;

		currentLayout[19][0] = RED;
		currentLayout[19][1] = RED;
		currentLayout[19][2] = RED;
		currentLayout[19][3] = RED;
		currentLayout[19][4] = RED;

		currentLayout[24][0] = WHITE;
		currentLayout[24][1] = WHITE;
	}

	void SetTestBoard2()
	{

		redBearOff = false;
		whiteBearOff = false;
		for (int x = 0; x < 25; x++)
		{
			for (int y = 0; y < 15; y++)
			{
				currentLayout[x][y] = NONE;
			}
		}

		currentLayout[2][0] = RED;
		currentLayout[2][1] = RED;

		currentLayout[3][0] = RED;
		
		currentLayout[4][0] = RED; 
		currentLayout[4][1] = RED;

		currentLayout[5][0] = RED;
		currentLayout[5][1] = RED;
		currentLayout[5][2] = RED;

		currentLayout[6][0] = RED;
		currentLayout[6][1] = RED;

		currentLayout[7][0] = WHITE;
		currentLayout[7][1] = WHITE;

		currentLayout[8][0] = WHITE;

		currentLayout[13][0] = RED;
		currentLayout[13][1] = RED;

		currentLayout[14][0] = WHITE;
		currentLayout[14][1] = WHITE;
		currentLayout[14][2] = WHITE;

		currentLayout[16][0] = RED;
		currentLayout[16][1] = RED;

		currentLayout[18][0] = WHITE;

		currentLayout[19][0] = WHITE;
		currentLayout[19][1] = WHITE;
		currentLayout[19][2] = WHITE;

		currentLayout[21][0] = WHITE;
		currentLayout[21][1] = WHITE;

		currentLayout[22][0] = WHITE;
		currentLayout[22][1] = WHITE;
		
		currentLayout[23][0] = WHITE;

		currentLayout[24][0] = WHITE;
	}

	void SetTestBoard3()
	{
		for (int x = 0; x < 25; x++)
		{
			for (int y = 0; y < 15; y++)
			{
				currentLayout[x][y] = NONE;
			}
		}

		currentLayout[1][0] = WHITE;

		currentLayout[4][0] = RED;
		currentLayout[4][1] = RED;
		currentLayout[4][2] = RED;

		currentLayout[5][0] = RED;
		currentLayout[5][1] = RED;

		currentLayout[6][0] = RED;
		currentLayout[6][1] = RED;
		currentLayout[6][2] = RED;

		currentLayout[7][0] = RED;
		currentLayout[7][1] = RED;

		currentLayout[8][0] = RED;

		currentLayout[13][0] = RED;
		currentLayout[13][1] = RED;

		currentLayout[14][0] = WHITE;

		currentLayout[15][0] = WHITE;
		currentLayout[15][1] = WHITE;
		currentLayout[15][2] = WHITE;

		currentLayout[17][0] = WHITE;
		currentLayout[17][1] = WHITE;

		currentLayout[18][0] = WHITE;
		currentLayout[18][1] = WHITE;

		currentLayout[19][0] = WHITE;
		currentLayout[19][1] = WHITE;
		currentLayout[19][2] = WHITE;
		currentLayout[19][3] = WHITE;

		currentLayout[20][0] = WHITE;
		currentLayout[20][1] = WHITE;

		currentLayout[24][0] = RED;
		currentLayout[24][1] = RED;
	}

	void SetTestBoard1()
	{

		for (int x = 0; x < 25; x++)
		{
			for (int y = 0; y < 15; y++)
			{
				currentLayout[x][y] = NONE;
			}
		}

		currentLayout[2][0] = RED;

		currentLayout[3][1] = RED;
		currentLayout[3][1] = RED;

		currentLayout[4][0] = WHITE;
		currentLayout[4][1] = WHITE;

		currentLayout[5][0] = RED;
		currentLayout[5][1] = RED;

		currentLayout[6][0] = RED;
		currentLayout[6][1] = RED;
		currentLayout[6][2] = RED;

		currentLayout[8][0] = RED;
		currentLayout[8][1] = RED;
		currentLayout[8][2] = RED;

		currentLayout[12][0] = WHITE;
		currentLayout[12][1] = WHITE;

		currentLayout[13][0] = RED;
		currentLayout[13][1] = RED;

		currentLayout[15][0] = RED;
		currentLayout[15][1] = RED;


		currentLayout[19][0] = WHITE;
		currentLayout[19][1] = WHITE;

		currentLayout[21][0] = WHITE;
		currentLayout[21][1] = WHITE;

		currentLayout[22][0] = WHITE;
		currentLayout[22][1] = WHITE;
		currentLayout[22][2] = WHITE;
		currentLayout[22][3] = WHITE;

		currentLayout[23][0] = WHITE;
		currentLayout[23][1] = WHITE;

		currentLayout[24][0] = WHITE;
	}

	
};

enum GAMEPHASE
{
	MAKING_ROLL,
	MAKING_MOVE
};

enum BACKGAMMONMOVETYPE
{
	NORMAL,
	HIT,
	BEAROFF,
	RETURNFROMBAR,
	RETURNFROMBARANDHIT, //why
	NONEAVAILABLE
};

enum MinMaxNext
{
	MIN, 
	MAX
};

struct BackgammonMove
{
	BACKGAMMONMOVETYPE type;

	int pointFrom;
	int checkerFrom;
	int pointTo;
	int checkerTo; 

	int diceUsed;
};

struct BackgammonMovePair
{
	int numberOfMoves;

	BackgammonMove first;
	BackgammonMove second;
	BackgammonMove third;
	BackgammonMove fourth;
};

struct PointOnBoard
{
	int pointNumber;
	SDL_Rect boundingBox;
};

struct Checker
{
	int team;
	Vector2D position;
	SDL_Rect boundingBox;
};


enum BACKGAMMONGAMESTATE
{
	START,
	ROLLDICE,
	PLAYERTURN,
	END,
	REDWIN,
	WHITEWIN,
};

#endif //BACKGAMMONCOMMONS_H