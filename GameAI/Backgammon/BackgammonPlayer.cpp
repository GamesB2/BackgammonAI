#include "BackgammonPlayer.h"
#include <iostream>
#include <SDL.h>
#include "../Texture2D.h"

using namespace::std;

//----------------------------------
//----------------------------------------------------------------

BackgammonPlayer::BackgammonPlayer(SDL_Renderer* renderer, BACKGAMMONCOLOUR colour, BackgammonBoard* board, SDL_Rect collisions[26])
{
	//Start values.
	for (int i = 0; i < 27; i++)
	{
		boardCollisionBoxes[i] = collisions[i];
	}

	mBackgammonBoard		= board;
	mTeamColour				= colour;

	if (mTeamColour == RED)
	{
		mEnemyColour = WHITE;
	}
	else if (mTeamColour == WHITE)
	{
		mEnemyColour = RED;
	}

	
}

//--------------------------------------------------------------------------------------------------

BackgammonPlayer::~BackgammonPlayer()
{
	mBackgammonBoard = NULL;

	mHighlightPositions = NULL;

	delete mSelectAPieceTexture;
	mSelectAPieceTexture = NULL;

	delete mSelectedPiecePosition;
	mSelectedPiecePosition = NULL;
}

//--------------------------------------------------------------------------------------------------



void BackgammonPlayer::DealWithTurn(SDL_Event e)
{

	if (mAllAvailableMoves.size() == 0)
	{
		GetAllMoveOptions(*mBackgammonBoard, mTeamColour, &mAllAvailableMoves);
		if (mAllAvailableMoves.size() == 0)
		{
			mBackgammonBoard->movesAvailable = 0;
			for (int i = 0; i < 4; i++)
			{
				mBackgammonBoard->diceAvailable[i] = 0;
			}
			return;
		}
	}

	if (e.type == SDL_MOUSEBUTTONUP)
	{
		DealWithMouseClick(e);
	}
}

bool BackgammonPlayer::DealWithMouseClick(SDL_Event e)
{
	//Check bounding boxes against mouseclick, loop through point to find friendly checkers, explore available moves if found
	//Since moving the end checker is no different from moving a middle checker, the last checker on a point is always selected

	SDL_Point mouseClick = SDL_Point();
	mouseClick.x = e.button.x;
	mouseClick.y = e.button.y;

	if (!mBackgammonBoard->pieceSelected)
	{
		if (mBackgammonBoard->redPiecesOnBar > 0 && mTeamColour == RED)
		{
			if (!SDL_PointInRect(&mouseClick, (const SDL_Rect*)&boardCollisionBoxes[0]))
			{
				cout << "Move Pieces From Bar First" << endl;
				mSelectedAvailableMoves.clear();
				return false;
			}
			else
			{
				pointSelected = 0;

				for each (BackgammonMove move in mAllAvailableMoves)
				{
					if (move.pointFrom == 0)
					{
						mSelectedAvailableMoves.push_back(move);
					}
				}
				mBackgammonBoard->pieceSelected = true;
				cout << "Selected point 0" << endl;
				return true;
			}
		}

		if (mBackgammonBoard->whitePiecesOnBar > 0 && mTeamColour == WHITE)
		{
			if (!SDL_PointInRect(&mouseClick, (const SDL_Rect*)&boardCollisionBoxes[0]))
			{
				cout << "Move Pieces From Bar First" << endl;
				mSelectedAvailableMoves.clear();
				return false;
			}
			else
			{
				pointSelected = 0;

				for each (BackgammonMove move in mAllAvailableMoves)
				{
					if (move.pointFrom == 0)
					{
						mSelectedAvailableMoves.push_back(move);
					}
				}
				mBackgammonBoard->pieceSelected = true;
				cout << "Selected point 0" << endl;
				return true;
			}
		}

		for (int i = 0; i < 26; i++)
		{
			if (SDL_PointInRect(&mouseClick, (const SDL_Rect*)&boardCollisionBoxes[i]))
			{
				for each (BackgammonMove move in mAllAvailableMoves)
				{
					if (move.pointFrom == i)
					{
						mSelectedAvailableMoves.push_back(move);			
						mBackgammonBoard->pieceSelected = true;
						cout << "Selected point " << i << endl;
					}
				}
				return true;
			}
		}

	}

	//Need to change this to use new vector move system
	if (mBackgammonBoard->pieceSelected)
	{
		for (int i = 0; i < 26; i++)
		{

			if (SDL_PointInRect(&mouseClick, (const SDL_Rect*)&boardCollisionBoxes[i]))
			{

				for each (BackgammonMove move in mSelectedAvailableMoves)
				{
					if (move.pointTo == i)
					{
						MakeAMove(mBackgammonBoard, &move);
						mSelectedAvailableMoves.clear();
						mAllAvailableMoves.clear();
						mBackgammonBoard->pieceSelected = false;

						cout << ColourAsString[(int)mTeamColour] << " moved " << move.pointFrom << " to " << move.pointTo << endl;
						return false;
					}

				}
				cout << "Moves cleared, select new piece" << endl;
				mSelectedAvailableMoves.clear();
				mBackgammonBoard->pieceSelected = false;
				return false;
			}
		}
	}
	return false;
}

void BackgammonPlayer::MakeAMove(BackgammonBoard * board, BackgammonMove * move)
{

	//TODO: Remove Dice

	switch (move->type)
	{
	case HIT:

		if (board->currentLayout[move->pointTo][move->checkerTo] == RED)
		{
			board->redPiecesOnBar++;
			board->currentLayout[move->pointTo][move->checkerTo] = board->currentLayout[move->pointFrom][move->checkerFrom];
			board->currentLayout[move->pointFrom][move->checkerFrom] = NONE;
		}
		else if (mBackgammonBoard->currentLayout[move->pointTo][move->checkerTo] == WHITE)
		{
			board->whitePiecesOnBar++;
			board->currentLayout[move->pointTo][move->checkerTo] = board->currentLayout[move->pointFrom][move->checkerFrom];
			board->currentLayout[move->pointFrom][move->checkerFrom] = NONE;
		}
		break;
	case NORMAL:
		board->currentLayout[move->pointTo][move->checkerTo] = board->currentLayout[move->pointFrom][move->checkerFrom];
		board->currentLayout[move->pointFrom][move->checkerFrom] = NONE;
		break;
	case RETURNFROMBAR:
		if (mTeamColour == RED)
		{
			board->redPiecesOnBar--;
			board->currentLayout[move->pointTo][move->checkerTo] = RED;
		}
		if (mTeamColour == WHITE)
		{
			board->whitePiecesOnBar--;
			board->currentLayout[move->pointTo][move->checkerTo] = WHITE;
		}
		break;
	case RETURNFROMBARANDHIT:
		if (mTeamColour == RED)
		{
			board->redPiecesOnBar--;
			board->currentLayout[move->pointTo][move->checkerTo] = RED;
			board->whitePiecesOnBar++;
		}
		if (mTeamColour == WHITE)
		{
			board->redPiecesOnBar++;
			board->currentLayout[move->pointTo][move->checkerTo] = WHITE;
			board->whitePiecesOnBar--;
		}
		break;
	case BEAROFF:
		if (mTeamColour == RED)
		{
			board->redPiecesOffBoard++;
		}
		if (mTeamColour == WHITE)
		{
			board->whitePiecesOffBoard++;
		}
		board->currentLayout[move->pointFrom][move->checkerFrom] = NONE;
		break;
	}

	//cout << "Move :" << move->pointFrom << " to " << move->pointTo << endl;

	for (int i = 3; i > -1; i--)
	{
		if (board->diceAvailable[i] == move->diceUsed)
		{
			board->diceAvailable[i] = 0;
			break;
		}
	}

	board->movesAvailable--;

	int redFound = 0;

	for (int i = 19; i < 25; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (board->currentLayout[i][j] == RED)
			{
				redFound++;
			}
		}
	}

	int whiteFound = 0;

	for (int i = 1; i < 7; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (board->currentLayout[i][j] == WHITE)
			{
				whiteFound++;
			}
		}
	}

	if (redFound + board->redPiecesOffBoard == 15)
	{
		board->redBearOff = true;
	}
	else
	{
		board->redBearOff = false;
	}

	if (whiteFound + board->whitePiecesOffBoard == 15)
	{
		board->whiteBearOff = true;
	}
	else
	{
		board->whiteBearOff = false;
	}

}

void BackgammonPlayer::GetAllMoveOptions(BackgammonBoard board, BACKGAMMONCOLOUR mTeamColour, vector<BackgammonMove>* currentMoves)
{
	int diceToCheck = 2;
	if (board.movesAvailable == 4 ||  board.movesAvailable == 3 || board.diceAvailable[1] == board.diceAvailable[0])
	{
		diceToCheck = 1;
	}

	if (mTeamColour == WHITE)
	{
		if (board.whitePiecesOnBar > 0)
		{
			for (int i = 0; i < diceToCheck; i++)
			{
				if (board.diceAvailable[i] == 0)
				{
					continue;
				}
				if (board.currentLayout[25 - board.diceAvailable[i]][0] == WHITE)
				{
					int found = 0;
					for (int k = 0; k < 16; k++)
					{
						if (board.currentLayout[25 - board.diceAvailable[i]][k] != mTeamColour)
						{
							found = k;
							break;
						}
					}

					BackgammonMove move;
					move.pointFrom = 0;
					move.checkerFrom = 0;
					move.pointTo = 25 - board.diceAvailable[i];
					move.checkerTo = found;
					move.type = RETURNFROMBAR;
					move.diceUsed = board.diceAvailable[i];
					currentMoves->push_back(move);
				}

				if (board.currentLayout[25 - board.diceAvailable[i]][0] == RED && board.currentLayout[25 - board.diceAvailable[i]][1] == NONE)
				{
					BackgammonMove move;
					move.pointFrom = 0;
					move.checkerFrom = 0;
					move.pointTo = 25 - board.diceAvailable[i];
					move.checkerTo = 0;
					move.type = RETURNFROMBARANDHIT;
					move.diceUsed = board.diceAvailable[i];
					currentMoves->push_back(move);
				}

				if (board.currentLayout[25 - board.diceAvailable[i]][0] == RED && board.currentLayout[25 - board.diceAvailable[i]][1] == RED)
				{
					continue;
				}

				if (board.currentLayout[25 - board.diceAvailable[i]][0] == NONE)
				{
					BackgammonMove move;
					move.pointFrom = 0;
					move.checkerFrom = 0;
					move.pointTo = 25 - board.diceAvailable[i];
					move.checkerTo = 0;
					move.type = RETURNFROMBAR;
					move.diceUsed = board.diceAvailable[i];
					currentMoves->push_back(move);
				}
			}
			return;
		}
	}

	if (mTeamColour == RED)
	{
		if (board.redPiecesOnBar > 0)
		{
			for (int i = 0; i < diceToCheck; i++)
			{
				if (board.diceAvailable[i] == 0)
				{
					continue;
				}
				if (board.currentLayout[0 + board.diceAvailable[i]][0] == RED)
				{
					int found = 0;
					for (int k = 0; k < 16; k++)
					{
						if (board.currentLayout[0 + board.diceAvailable[i]][k] != mTeamColour || k == 15)
						{
							found = k;
							break;
						}
					}

					BackgammonMove move;
					move.pointFrom = 0;
					move.checkerFrom = 0;
					move.pointTo = 0 + board.diceAvailable[i];
					move.checkerTo = found;
					move.type = RETURNFROMBAR;
					move.diceUsed = board.diceAvailable[i];
					currentMoves->push_back(move);
				}

				if (board.currentLayout[0 + board.diceAvailable[i]][0] == WHITE && board.currentLayout[0 + board.diceAvailable[i]][1] == NONE)
				{
					BackgammonMove move;
					move.pointFrom = 0;
					move.checkerFrom = 0;
					move.pointTo = 0 + board.diceAvailable[i];
					move.checkerTo = 0;
					move.type = RETURNFROMBARANDHIT;
					move.diceUsed = board.diceAvailable[i];
					currentMoves->push_back(move);
				}

				if (board.currentLayout[0 + board.diceAvailable[i]][0] == WHITE && board.currentLayout[0 + board.diceAvailable[i]][1] == WHITE)
				{
					continue;
				}

				if (board.currentLayout[0 + board.diceAvailable[i]][0] == NONE)
				{
					BackgammonMove move;
					move.pointFrom = 0;
					move.checkerFrom = 0;
					move.pointTo = 0 + board.diceAvailable[i];
					move.checkerTo = 0;
					move.type = RETURNFROMBAR;
					move.diceUsed = board.diceAvailable[i];
					currentMoves->push_back(move);
				}
			}
			return;
		}
	}

	for (int i = 1; i < 25; i++)
	{
		if (board.currentLayout[i][0] == mTeamColour)
		{
			int fromFound;
			for (int j = 0; j < 16; j++)
			{
				if (board.currentLayout[i][j] != mTeamColour)
				{
					fromFound = j -1;
					break;
				}
			}

			if (mTeamColour == RED)
			{
				for (int j = 0; j < diceToCheck; j++)
				{
					if (i + board.diceAvailable[j] < 25)
					{
						if (board.diceAvailable[j] == 0)
						{
							continue;
						}
						if (board.currentLayout[i + board.diceAvailable[j]][0] == RED)
						{
							int found = 0;
							for (int k = 0; k < 15; k++)
							{
								if (board.currentLayout[i + board.diceAvailable[j]][k] != mTeamColour || j == 15)
								{
									found = k;
									break;
								}
							}

							BackgammonMove move;
							move.pointFrom = i;
							move.checkerFrom = fromFound;
							move.pointTo = i + board.diceAvailable[j];
							move.checkerTo = found;
							move.type = NORMAL;
							move.diceUsed = board.diceAvailable[j];
							currentMoves->push_back(move);
						}

						if (board.currentLayout[i + board.diceAvailable[j]][0] == WHITE && board.currentLayout[i + board.diceAvailable[j]][1] == WHITE)
						{
							continue;
						}

						if (board.currentLayout[i + board.diceAvailable[j]][0] == WHITE && board.currentLayout[i + board.diceAvailable[j]][1] == NONE)
						{
							BackgammonMove move;
							move.pointFrom = i;
							move.checkerFrom = fromFound;
							move.pointTo = i + board.diceAvailable[j];
							move.checkerTo = 0;
							move.type = HIT;
							move.diceUsed = board.diceAvailable[j];
							currentMoves->push_back(move);
						}
						if (board.currentLayout[i + board.diceAvailable[j]][0] == NONE)
						{
							BackgammonMove move;
							move.pointFrom = i;
							move.checkerFrom = fromFound;
							move.pointTo = i + board.diceAvailable[j];
							move.checkerTo = 0;
							move.type = NORMAL;
							move.diceUsed = board.diceAvailable[j];
							currentMoves->push_back(move);
						}
					}
				}

			}

			if (mTeamColour == WHITE)
			{
				for (int j = 0; j < diceToCheck; j++)
				{
					if (i - board.diceAvailable[j] > 0)
					{
						if (board.diceAvailable[j] == 0)
						{
							continue;
						}
						if (board.currentLayout[i - board.diceAvailable[j]][0] == WHITE)
						{
							int found = 0;
							for (int k = 0; k < 15; k++)
							{
								if (board.currentLayout[i - board.diceAvailable[j]][k] != mTeamColour)
								{
									found = k;
									break;
								}
							}

							BackgammonMove move;
							move.pointFrom = i;
							move.checkerFrom = fromFound;
							move.pointTo = i - board.diceAvailable[j];
							move.checkerTo = found;
							move.type = NORMAL;
							move.diceUsed = board.diceAvailable[j];
							currentMoves->push_back(move);
						}

						if (board.currentLayout[i - board.diceAvailable[j]][0] == RED && board.currentLayout[i - board.diceAvailable[j]][1] == RED)
						{
							continue;
						}

						if (board.currentLayout[i - board.diceAvailable[j]][0] == RED && board.currentLayout[i - board.diceAvailable[j]][1] == NONE)
						{
							BackgammonMove move;
							move.pointFrom = i;
							move.checkerFrom = fromFound;
							move.pointTo = i - board.diceAvailable[j];
							move.checkerTo = 0;
							move.type = HIT;
							move.diceUsed = board.diceAvailable[j];
							currentMoves->push_back(move);
						}
						if (board.currentLayout[i - board.diceAvailable[j]][0] == NONE)
						{
							BackgammonMove move;
							move.pointFrom = i;
							move.checkerFrom = fromFound;
							move.pointTo = i - board.diceAvailable[j];
							move.checkerTo = 0;
							move.type = NORMAL;
							move.diceUsed = board.diceAvailable[j];
							currentMoves->push_back(move);
						}
					}
				}
			}
		}
	}

	if (board.redBearOff && mTeamColour == RED)
	{
		bool found = false;
	
		for (int i = 0; i < diceToCheck; i++)
		{
			if (board.diceAvailable[i] == 0)
			{
				continue;
			}
			if (board.currentLayout[25 - board.diceAvailable[i]][0] == RED)
			{
				int fromFound = 0;
				for (int j = 0; j < 16; j++)
				{
					if (board.currentLayout[25 - board.diceAvailable[i]][j] == NONE || j == 15)
					{
						BackgammonMove move;
						move.pointFrom =  25 - board.diceAvailable[i];
						move.checkerFrom = fromFound -1;
						move.pointTo = 25;
						move.checkerTo = 0;
						move.type = BEAROFF;
						move.diceUsed = board.diceAvailable[i];
						currentMoves->push_back(move);
						found = true;
						break;
					}
					fromFound++;
				}
			}
		}

		if (found == false)
		{
			int highestDice = board.diceAvailable[0];
			for (int i = 0; i < diceToCheck; i++)
			{
				if (board.diceAvailable[i] > highestDice)
				{
					highestDice = board.diceAvailable[i];
				}
			}

			for (int j = 19; j < 25; j++)
			{
				if (board.currentLayout[j][0] == RED)
				{
					int fromFound = 0;
					for (int k = 0; k < 16; k++)
					{
						if (board.currentLayout[j][k] == NONE || j == 15)
						{
							BackgammonMove move;
							move.pointFrom = j;
							move.checkerFrom = fromFound -1;
							move.pointTo = 25;
							move.checkerTo = 0;
							move.type = BEAROFF;
							move.diceUsed = highestDice;
							currentMoves->push_back(move);
							break;
						}
						fromFound++;
					}
					break;
				}
			}
		}
	}


	if (board.whiteBearOff && mTeamColour == WHITE)
	{
		bool found = false;

		for (int i = 0; i < diceToCheck; i++)
		{
			if (board.diceAvailable[i] == 0)
			{
				continue;
			}
			if (board.currentLayout[board.diceAvailable[i]][0] == WHITE)
			{
				int fromFound = 0;
				for (int j = 0; j < 16; j++)
				{
					if (board.currentLayout[board.diceAvailable[i]][j] == NONE || j == 15)
					{
						BackgammonMove move;
						move.pointFrom = board.diceAvailable[i];
						move.checkerFrom = fromFound -1;
						move.pointTo = 25;
						move.checkerTo = 0;
						move.type = BEAROFF;
						move.diceUsed = board.diceAvailable[i];
						currentMoves->push_back(move);
						found = true;
						break;
					}
					fromFound++;
				}
			}
		}

		if (found == false)
		{
			int highestDice = board.diceAvailable[0];
			for (int i = 0; i < diceToCheck; i++)
			{
				if (board.diceAvailable[i] > highestDice)
				{
					highestDice = board.diceAvailable[i];
				}
			}

			for (int j = 6; j > 0; j--)
			{
				if (board.currentLayout[j][0] == WHITE)   
				{
					int fromFound = 0;
					for (int k = 0; k < 16; k++)
					{
						if (board.currentLayout[j][k] == NONE || j == 15)
						{
							BackgammonMove move;
							move.pointFrom = j;
							move.checkerFrom = fromFound -1;
							move.pointTo = 25;
							move.checkerTo = 0;
							move.type = BEAROFF;
							move.diceUsed = highestDice;
							currentMoves->push_back(move);
							break;
						}
						fromFound++;
					}
					break;
				}
			}
		}
	}
}

bool BackgammonPlayer::DiceRoll(SDL_Event e)
{

	switch (e.type)
	{
	case SDL_MOUSEBUTTONUP:
	{
		return true;
	}
	}

	return false;
}

//--------------------------------------------------------------------------------------------------
