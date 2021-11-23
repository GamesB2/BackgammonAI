//#include "BackgammonMoveManager.h"
//#include "BackgammonPlayer.h"
//
//BackgammonMoveManager * BackgammonMoveManager::mInstance = nullptr;
//
//BackgammonMoveManager::BackgammonMoveManager()
//{
//}
//
//
//BackgammonMoveManager::~BackgammonMoveManager()
//{
//}
//
//BackgammonMoveManager * BackgammonMoveManager::Instance()
//{
//	if (mInstance == nullptr)
//	{
//		mInstance = new BackgammonMoveManager();
//	}
//	return mInstance;
//}
//
//
//
//void BackgammonMoveManager::StoreAvailableMoves(int pointFrom, BackgammonBoard* board, BACKGAMMONCOLOUR player, BACKGAMMONCOLOUR opponent)
//{
//	
//}
//
//bool BackgammonMoveManager::AttemptToMakeMove(SDL_Event e, BackgammonBoard * mBackgammonBoard, BACKGAMMONCOLOUR mTeamColour, BACKGAMMONCOLOUR mEnemyColour)
//{
//	//This is only called when a piece is currently selected
//	//Check mouse position against collision rectangles, e.g. Collisions[1] associates with currentLayout[1]
//	//If collision number is the same as noted in pairArr, make the move
//	SDL_Point mouseClick = SDL_Point();
//	mouseClick.x = e.button.x;
//	mouseClick.y = e.button.y;
//
//	for (int i = 0; i < 25; i++)
//	{
//
//		if (SDL_PointInRect(&mouseClick, &mBackgammonBoard->boardCollisionBoxes[i]))
//		{
//			cout << i << " position has been clicked" << endl;
//			int lastChecker = -1;
//
//			if (i == mBackgammonBoard->availablePointOne[0])
//			{
//				if (mBackgammonBoard->currentLayout[i][0] == mEnemyColour)
//				{
//					mBackgammonBoard->currentLayout[i][0] = mTeamColour;
//					if (mEnemyColour == RED)
//					{
//						mBackgammonBoard->redPiecesOnBar++;
//					}
//					else if (mEnemyColour == WHITE)
//					{
//						mBackgammonBoard->whitePiecesOnBar++;
//					}
//					mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]] = NONE;
//				}
//				else
//				{
//					mBackgammonBoard->currentLayout[i][mBackgammonBoard->availablePointOne[1]] = mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]];
//					mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]] = NONE;
//				}
//
//				if (mBackgammonBoard->movesAvailable <= 2)
//				{
//					mBackgammonBoard->diceAvailable[0] = -1;
//				}
//				cout << "Piece moved" << endl;
//				mBackgammonBoard->movesAvailable--;
//
//				PrepareForNextMove(mBackgammonBoard, mTeamColour, mEnemyColour);
//
//				return mBackgammonBoard->pieceSelected = false;
//			}
//			else if (i == mBackgammonBoard->availablePointTwo[0])
//			{
//				if (mBackgammonBoard->currentLayout[i][0] == mEnemyColour)
//				{
//					mBackgammonBoard->currentLayout[i][0] = mTeamColour;
//					if (mEnemyColour == RED)
//					{
//						mBackgammonBoard->redPiecesOnBar++;
//					}
//					else if (mEnemyColour == WHITE)
//					{
//						mBackgammonBoard->whitePiecesOnBar++;
//					}
//					mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]] = NONE;
//				}
//				else
//				{
//					mBackgammonBoard->currentLayout[i][mBackgammonBoard->availablePointTwo[1]] = mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]];
//					mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]] = NONE;
//				}
//				
//				mBackgammonBoard->availablePointTwo[0] = -1;
//				mBackgammonBoard->availablePointTwo[1] = -1;
//
//				if (mBackgammonBoard->movesAvailable <= 2)
//				{
//					mBackgammonBoard->diceAvailable[1] = -1;
//
//				}
//				cout << "Piece moved" << endl;
//				mBackgammonBoard->movesAvailable--;
//
//				PrepareForNextMove(mBackgammonBoard, mTeamColour, mEnemyColour);
//
//				return mBackgammonBoard->pieceSelected = false;
//			}
//			else
//			{
//				cout << "Not an available move" << endl;
//			}
//		}
//
//	}
//
//	return mBackgammonBoard->pieceSelected = false;
//}
//
//bool BackgammonMoveManager::AttemptBearOff(SDL_Event e, BackgammonBoard * mBackgammonBoard, BACKGAMMONCOLOUR mTeamColour, BACKGAMMONCOLOUR mEnemyColour)
//{
//	SDL_Point mouseClick = SDL_Point();
//	mouseClick.x = e.button.x;
//	mouseClick.y = e.button.y;
//
//	if (SDL_PointInRect(&mouseClick, &mBackgammonBoard->boardCollisionBoxes[25]))
//	{
//		mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]] = NONE;
//		if (mTeamColour == RED)
//		{
//			mBackgammonBoard->redPiecesOffBoard++;
//		}
//		else if (mTeamColour == WHITE)
//		{
//			mBackgammonBoard->whitePiecesOffBoard++;
//		}
//	}
//	else
//	{
//		AttemptToMakeMove(e, mBackgammonBoard, mTeamColour, mEnemyColour);
//	}
//
//	return true;
//}
//
//bool BackgammonMoveManager::AttemptToMakeMoveFromBar(SDL_Event e, BackgammonBoard * mBackgammonBoard, BACKGAMMONCOLOUR mTeamColour, BACKGAMMONCOLOUR mEnemyColour)
//{
//	//This is only called when a piece is currently selected
//	//Check mouse position against collision rectangles, e.g. Collisions[1] associates with currentLayout[1]
//	//If collision number is the same as noted in pairArr, make the move
//	SDL_Point mouseClick = SDL_Point();
//	mouseClick.x = e.button.x;
//	mouseClick.y = e.button.y;
//
//	for (int i = 0; i < 25; i++)
//	{
//
//		if (SDL_PointInRect(&mouseClick, &mBackgammonBoard->boardCollisionBoxes[i]))
//		{
//			cout << i << " position has been clicked" << endl;
//			int lastChecker = -1;
//
//			if (i == mBackgammonBoard->availablePointOne[0])
//			{
//				if (mBackgammonBoard->currentLayout[i][0] == mEnemyColour)
//				{
//					mBackgammonBoard->currentLayout[i][0] = mTeamColour;
//					if (mEnemyColour == RED)
//					{
//						mBackgammonBoard->redPiecesOnBar++;
//					}
//					else if (mEnemyColour == WHITE)
//					{
//						mBackgammonBoard->whitePiecesOnBar++;
//					}
//					mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]] = NONE;
//				}
//				else
//				{
//					mBackgammonBoard->currentLayout[i][mBackgammonBoard->availablePointOne[1]] = mTeamColour;
//				}
//
//				if (mBackgammonBoard->movesAvailable <= 2)
//				{
//					mBackgammonBoard->diceAvailable[0] = -1;
//				}
//				cout << "Piece moved" << endl;
//				mBackgammonBoard->movesAvailable--;
//
//				if (mTeamColour == RED)
//				{
//					mBackgammonBoard->redPiecesOnBar--;
//				}
//				else if (mTeamColour == WHITE)
//				{
//					mBackgammonBoard->whitePiecesOnBar--;
//				}
//
//				PrepareForNextMove(mBackgammonBoard, mTeamColour, mEnemyColour);
//
//				return mBackgammonBoard->pieceSelected = false;
//			}
//			else if (i == mBackgammonBoard->availablePointTwo[0])
//			{
//				if (mBackgammonBoard->currentLayout[i][0] == mEnemyColour)
//				{
//					mBackgammonBoard->currentLayout[i][0] = mTeamColour;
//					if (mEnemyColour == RED)
//					{
//						mBackgammonBoard->redPiecesOnBar++;
//					}
//					else if (mEnemyColour == WHITE)
//					{
//						mBackgammonBoard->whitePiecesOnBar++;
//					}
//					mBackgammonBoard->currentLayout[mBackgammonBoard->pieceSelectedArrayPos[0]][mBackgammonBoard->pieceSelectedArrayPos[1]] = NONE;
//				}
//				else
//				{
//					mBackgammonBoard->currentLayout[i][mBackgammonBoard->availablePointTwo[1]] = mTeamColour;
//				}
//
//				mBackgammonBoard->availablePointTwo[0] = -1;
//				mBackgammonBoard->availablePointTwo[1] = -1;
//
//				if (mBackgammonBoard->movesAvailable <= 2)
//				{
//					mBackgammonBoard->diceAvailable[1] = -1;
//
//				}
//				cout << "Piece moved" << endl;
//				mBackgammonBoard->movesAvailable--;
//
//
//				if (mTeamColour == RED)
//				{
//					mBackgammonBoard->redPiecesOnBar--;
//				}
//				else if (mTeamColour == WHITE)
//				{
//					mBackgammonBoard->whitePiecesOnBar--;
//				}
//
//				PrepareForNextMove(mBackgammonBoard, mTeamColour, mEnemyColour);
//
//				return mBackgammonBoard->pieceSelected = false;
//			}
//			else
//			{
//				cout << "Not an available move" << endl;
//			}
//		}
//
//	}
//
//	return mBackgammonBoard->pieceSelected = false;
//}
//
//void BackgammonMoveManager::PrepareForNextMove(BackgammonBoard * board, BACKGAMMONCOLOUR player, BACKGAMMONCOLOUR opponent)
//{
//	for (int i = 0; i < 2; i++)
//	{
//		board->availablePointOne[i] = -1;
//		board->availablePointTwo[i] = -1;
//	}
//
//	board->pieceSelected = false;
//
//	int count = 0;
//
//	for (int x = 19; x < 25; x++)
//	{
//		for (int y = 0; y < 15; y++)
//		{
//			if (board->currentLayout[x][y] == RED)
//			{
//				count++;
//			}
//		}
//	}
//
//	count += board->redPiecesOffBoard;
//
//	if (count == 15)
//	{
//		board->redBearOff = true;
//	}
//	else
//	{
//		board->redBearOff = false;
//	}
//
//	count = 0;
//
//	for (int x = 1; x < 7; x++)
//	{
//		for (int y = 0; y < 15; y++)
//		{
//			if (board->currentLayout[x][y] == WHITE)
//			{
//				count++;
//			}
//		}
//	}
//
//	count += board->whitePiecesOffBoard;
//
//	if (count == 15)
//	{
//		board->whiteBearOff = true;
//	}
//	else
//	{
//		board->whiteBearOff = false;
//	}
//
//	//Check if more moves avaialble
//}