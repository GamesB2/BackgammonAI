#include "GameScreen_Backgammon.h"
#include <iostream>
#include <time.h>
#include "../Texture2D.h"
#include "BackgammonConstants.h"

using namespace::std;

GameScreen_Backgammon::GameScreen_Backgammon(SDL_Renderer* renderer) : GameScreen(renderer)
{
	srand(time(NULL));

	//Board essentials
	mBackgroundTexture = new Texture2D(renderer);
	mBackgroundTexture->LoadFromFile(kBackgammonBoardPath);
	mRedChecker = new Texture2D(renderer);
	mRedChecker->LoadFromFile(kBackgammonRedCheckerPath);
	mWhiteChecker = new Texture2D(renderer);
	mWhiteChecker->LoadFromFile(kBackgammonWhiteCheckerPath);

	//Dice
	mDiceOne = new Texture2D(renderer);
	mDiceOne->LoadFromFile(kBackgammonDiceOne);
	mDiceTwo = new Texture2D(renderer);
	mDiceTwo->LoadFromFile(kBackgammonDiceTwo);
	mDiceThree = new Texture2D(renderer);
	mDiceThree->LoadFromFile(kBackgammonDiceThree);
	mDiceFour = new Texture2D(renderer);
	mDiceFour->LoadFromFile(kBackgammonDiceFour);
	mDiceFive = new Texture2D(renderer);
	mDiceFive->LoadFromFile(kBackgammonDiceFive);
	mDiceSix = new Texture2D(renderer);
	mDiceSix->LoadFromFile(kBackgammonDiceSix);

	//Move Highlights
	mSelectedPieceHighlight = new Texture2D(renderer);
	mSelectedPieceHighlight->LoadFromFile(kBackgammonPieceHighlight);
	mAvailableMoveHighlight = new Texture2D(renderer);
	mAvailableMoveHighlight->LoadFromFile(kBackgammonMoveHighlight);
	mBearOffHighlight = new Texture2D(renderer);
	mBearOffHighlight->LoadFromFile(kBackgammonBearOffHighlight);

	//Numbers
	mNumberSpriteSheet = new Texture2D(renderer);
	mNumberSpriteSheet->LoadFromFile(kBackgammonNumbers);
	
	//Initialisation of variables to be used
	//Since you cannot have negative checkers and cannot move negative spaces, negative numbers are used when something does not currently exist

	piecesOnBar = 0;

	mPlayerTurn = 0;
	mCurrentGameState = ROLLDICE;

	mRenderer = renderer;

	//Bar Bounding Box
	SDL_Rect boardPosition = SDL_Rect();
	boardPosition.x = (kBackgammonBoardWidth / 2) - 10;
	boardPosition.y = (100);
	boardPosition.h = (300);
	boardPosition.w = 25;

	boardCollisionBoxes[0] = boardPosition;

	//Top Right Quarter
	for (int x = 1; x < 7; x++)
	{
		SDL_Rect boardPosition = SDL_Rect();
		boardPosition.x = int(kBackgammonBoardWidth - (kBackgammonBoardWidth / 2) / 6.5 * x - 10);
		boardPosition.y = 15;
		boardPosition.h = 200;
		boardPosition.w = 50;

		boardCollisionBoxes[x] = boardPosition;
	}
	//Top left quarter
	for (int x = 7; x < 13; x++)
	{

		SDL_Rect boardPosition = SDL_Rect();
		boardPosition.x = int((kBackgammonBoardWidth / 2) - (kBackgammonBoardWidth / 2) / 6.5 * (x - 6) - 10);
		boardPosition.y = 15;
		boardPosition.h = 200;
		boardPosition.w = 50;

		boardCollisionBoxes[x] = boardPosition;
	}
	//Bottom left quarter 
	for (int x = 13; x < 19; x++)
	{
		SDL_Rect boardPosition = SDL_Rect();
		boardPosition.x = int(((kBackgammonBoardWidth / 2) / 6.5) * (x - 12) - mWhiteChecker->GetWidth());
		boardPosition.y = 252;
		boardPosition.h = 250;
		boardPosition.w = 50;

		boardCollisionBoxes[x] = boardPosition;
		
	}
	//Bottom right quarter 
	for (int x = 19; x < 25; x++)
	{
		Vector2D pos = Vector2D((kBackgammonBoardWidth / 2) + ((kBackgammonBoardWidth / 2) / 6.5) * (x - 18) - mWhiteChecker->GetWidth(), kBackgammonBoardHeight / 3 * 2 - mRedChecker->GetHeight());
		SDL_Rect boardPosition = SDL_Rect();
		boardPosition.x = int((kBackgammonBoardWidth / 2) + ((kBackgammonBoardWidth / 2) / 6.5) * (x - 18) - mWhiteChecker->GetWidth());
		boardPosition.y = 252;
		boardPosition.h = 250;
		boardPosition.w = 50;

		boardCollisionBoxes[x] = boardPosition;
	}
	
	//Bear off Bounding Box

	boardPosition = SDL_Rect();
	boardPosition.x = 15;
	boardPosition.y = kBackgammonBoardHeight / 6 * 4.65;
	boardPosition.h = 145;
	boardPosition.w = 270;

	boardCollisionBoxes[25] = boardPosition;

	mBackgammonBoard = new BackgammonBoard();
	mBackgammonBoard->SetTestBoard3();

	mPlayers[0] = new BackgammonPlayer(renderer, RED, mBackgammonBoard, boardCollisionBoxes);
	mPlayers[1] = new BackgammonPlayerAI(renderer, WHITE, mBackgammonBoard, boardCollisionBoxes);

	cout << "Starting Backgammon" << endl;
	cout << "AI player using Expectimax search" << endl;
	cout << "Press D to see debug output, this will make the AI run very slowly!" << endl;
		 
}

GameScreen_Backgammon::~GameScreen_Backgammon()
{

}

void GameScreen_Backgammon::Update(size_t deltaTime, SDL_Event e)
{

	if (e.type == SDL_KEYDOWN)
	{

		switch (e.key.keysym.sym)
		{
		case SDLK_d:
			if (BackgammonPlayerAI* player = dynamic_cast<BackgammonPlayerAI*>(mPlayers[1]))
			{
				player->SetDebug();
			}
			break;
		case SDLK_b:
			cout << "Select test board from value range 1 - 4. Select 0 to cancel operation" << endl;

			int select;

			cin >> select;

			switch (select)
			{
			case 0:
				break;
			case 1:
				mBackgammonBoard->SetTestBoard1();
				break;
			case 2:
				mBackgammonBoard->SetTestBoard2();
				break;
			case 3:
				mBackgammonBoard->SetTestBoard3();
				break;
			case 4: 
				break;
			}
			break;
		case SDLK_1:
			if (BackgammonPlayerAI* player = dynamic_cast<BackgammonPlayerAI*>(mPlayers[1]))
			{
				player->SetAI(1);
			}
			break;
		case SDLK_2:
			if (BackgammonPlayerAI* player = dynamic_cast<BackgammonPlayerAI*>(mPlayers[1]))
			{
				player->SetAI(2);
			}
			break;
		case SDLK_3:
			if (BackgammonPlayerAI* player = dynamic_cast<BackgammonPlayerAI*>(mPlayers[1]))
			{
				player->SetAI(3);
			}
			break;
		}
	}

		if (mBackgammonBoard->whitePiecesOffBoard == 15)
		{
			mCurrentGameState = WHITEWIN;
		}
		if (mBackgammonBoard->redPiecesOffBoard == 15)
		{
			mCurrentGameState = REDWIN;
		}

		switch (mCurrentGameState)
		{
		case START:
			mCurrentGameState = ROLLDICE;
			break;
		case ROLLDICE:
			if (mPlayers[mPlayerTurn]->DiceRoll(e))
			{
				SetDice();
			}
			break;
		case PLAYERTURN:
			mPlayers[mPlayerTurn]->DealWithTurn(e);
			if (mBackgammonBoard->movesAvailable == 0)
			{
				mCurrentGameState = END;
			}
			break;
		case END:
			if (mPlayerTurn == 0) mPlayerTurn = 1;
			else if (mPlayerTurn == 1) mPlayerTurn = 0;
			mCurrentGameState = START;
			break;
		case REDWIN:
			//Output REDWIN render
			//if e.click, reset
			break;
		case WHITEWIN:
			//Output WHITEWIN render
			//if e.click, reset
			break;
		}


	}

void GameScreen_Backgammon::SetDice()
{

	mBackgammonBoard->diceAvailable[0] = rand() % 6 + 1;
	mBackgammonBoard->diceAvailable[1] = rand() % 6 + 1;
	mBackgammonBoard->diceAvailable[2] = 0;
	mBackgammonBoard->diceAvailable[3] = 0;
	mBackgammonBoard->movesAvailable = 2;

	if (mBackgammonBoard->diceAvailable[1] == mBackgammonBoard->diceAvailable[0])
	{
		mBackgammonBoard->diceAvailable[2] = mBackgammonBoard->diceAvailable[3] = mBackgammonBoard->diceAvailable[1];
		mBackgammonBoard->movesAvailable = 4;
	}

	mCurrentGameState = PLAYERTURN;
}


void GameScreen_Backgammon::Render()
{
	mBackgroundTexture->Render(Vector2D());

	RenderPieces();

	Vector2D dicePos = Vector2D();

	for (int i = 0; i < 2; i++)
	{
		switch (mBackgammonBoard->diceAvailable[i])
		{
		case 1:
			dicePos = Vector2D((kBackgammonBoardWidth / 4) * (i + 2), kDiceYPos);
			mDiceOne->Render(dicePos, SDL_FLIP_NONE, 0);
			break;
		case 2:
			dicePos = Vector2D((kBackgammonBoardWidth / 4) * (i + 2), kDiceYPos);
			mDiceTwo->Render(dicePos, SDL_FLIP_NONE, 0);
			break;
		case 3:
			dicePos = Vector2D((kBackgammonBoardWidth / 4) * (i + 2), kDiceYPos);
			mDiceThree->Render(dicePos, SDL_FLIP_NONE, 0);
			break;
		case 4:
			dicePos = Vector2D((kBackgammonBoardWidth / 4) * (i + 2), kDiceYPos);
			mDiceFour->Render(dicePos, SDL_FLIP_NONE, 0);
			break;
		case 5:
			dicePos = Vector2D((kBackgammonBoardWidth / 4) * (i + 2), kDiceYPos);
			mDiceFive->Render(dicePos, SDL_FLIP_NONE, 0);
			break;
		case 6:
			dicePos = Vector2D((kBackgammonBoardWidth / 4) * (i + 2), kDiceYPos);
			mDiceSix->Render(dicePos, SDL_FLIP_NONE, 0);
			break;
		case 0:
			break;
		}
	}
}

void GameScreen_Backgammon::RenderPieces()
{

	//Loops through current board array and renders checker positions, dodgy maths that needs to be tweaked at later date
	//Uses pieceSelected array and availablePosition ints to render highlights, blue for selected and green for potential move 

	//Top right quarter
	for (int x = 1; x < 7; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			if (mBackgammonBoard->currentLayout[x][y] == RED)
			{
				Vector2D pos = Vector2D((kBackgammonBoardWidth - 6) - ((280 / 6) * x), ((15)) + mRedChecker->GetHeight() * y);

				mRedChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}
			if (mBackgammonBoard->currentLayout[x][y] == WHITE)
			{
				Vector2D pos = Vector2D((kBackgammonBoardWidth - 6) - ((280 / 6) * x), ((15)) + mWhiteChecker->GetHeight() * y);

				mWhiteChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}
			if (mPlayers[mPlayerTurn]->mSelectedAvailableMoves.size() > 0)
			{
				for each (BackgammonMove move in mPlayers[mPlayerTurn]->mSelectedAvailableMoves)
				{
					if (move.pointTo == x && move.checkerTo == y)
					{
						Vector2D pos = Vector2D((kBackgammonBoardWidth - 6) - ((280 / 6) * x), ((15)) + mWhiteChecker->GetHeight() * y);

						mAvailableMoveHighlight->Render(pos, SDL_FLIP_NONE, 0.0);
					}
				}
			}
		}
	}
	//Top left quarter
	for (int x = 7; x < 13; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			if (mBackgammonBoard->currentLayout[x][y] == RED)
			{
				Vector2D pos = Vector2D((kBackgammonBoardWidth / 2 - 55) - (46.6 * (x - 7)), ((15)) + mRedChecker->GetHeight() * y);

				mRedChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}
			if (mBackgammonBoard->currentLayout[x][y] == WHITE)
			{
				Vector2D pos = Vector2D((kBackgammonBoardWidth / 2 - 55) - (46.6 * (x - 7)), ((15)) + mWhiteChecker->GetHeight() * y);

				mWhiteChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}
			if (mPlayers[mPlayerTurn]->mSelectedAvailableMoves.size() > 0)
			{
				for each (BackgammonMove move in mPlayers[mPlayerTurn]->mSelectedAvailableMoves)
				{
					if (move.pointTo == x && move.checkerTo == y)
					{
						Vector2D pos = Vector2D((kBackgammonBoardWidth / 2 - 55) - (46.6 * (x - 7)), ((15)) + mWhiteChecker->GetHeight() * y);

						mAvailableMoveHighlight->Render(pos, SDL_FLIP_NONE, 0.0);
					}
				}
			}
		}
	}
	//Bottom left quarter 
	for (int x = 13; x < 19; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			if (mBackgammonBoard->currentLayout[x][y] == RED)
			{
				Vector2D pos = Vector2D(20 + ((280 / 6) * (x - 13)), (kBackgammonBoardHeight / 3 * 2) + 18 - mRedChecker->GetHeight() * y);

				mRedChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}
			if (mBackgammonBoard->currentLayout[x][y] == WHITE)
			{
				Vector2D pos = Vector2D(20 + ((280 / 6) * (x - 13)), (kBackgammonBoardHeight / 3 * 2) + 18 - mWhiteChecker->GetHeight() * y);

				mWhiteChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}
			if (mPlayers[mPlayerTurn]->mSelectedAvailableMoves.size() > 0)
			{
				for each (BackgammonMove move in mPlayers[mPlayerTurn]->mSelectedAvailableMoves)
				{
					if (move.pointTo == x && move.checkerTo == y)
					{
						Vector2D pos = Vector2D(20 + ((280 / 6) * (x - 13)), (kBackgammonBoardHeight / 3 * 2) + 18 - mWhiteChecker->GetHeight() * y);

						mAvailableMoveHighlight->Render(pos, SDL_FLIP_NONE, 0.0);
					}
				}
			}
		}
	}
	//Bottom right quarter 
	for (int x = 19; x < 25; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			if (mBackgammonBoard->currentLayout[x][y] == RED)
			{
				Vector2D pos = Vector2D((kBackgammonBoardWidth / 2 + 22) + (46.6 * (x - 19)), (kBackgammonBoardHeight / 3 * 2) + 18 - mRedChecker->GetHeight() * y);

				mRedChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}
			if (mBackgammonBoard->currentLayout[x][y] == WHITE)
			{
				Vector2D pos = Vector2D((kBackgammonBoardWidth / 2 + 22) + (46.6 * (x - 19)), (kBackgammonBoardHeight / 3 * 2) + 18 - mWhiteChecker->GetHeight() * y);

				mWhiteChecker->Render(pos, SDL_FLIP_NONE, 0.0);
			}

			if (mPlayers[mPlayerTurn]->mSelectedAvailableMoves.size() > 0)
			{
				for each (BackgammonMove move in mPlayers[mPlayerTurn]->mSelectedAvailableMoves)
				{
					if (move.pointTo == x && move.checkerTo == y)
					{
						Vector2D pos = Vector2D((kBackgammonBoardWidth / 2 + 22) + (46.6 * (x - 19)), (kBackgammonBoardHeight / 3 * 2) + 18 - mWhiteChecker->GetHeight() * y);

						mAvailableMoveHighlight->Render(pos, SDL_FLIP_NONE, 0.0);
					}
				}
			}
		}
	}

	if (mBackgammonBoard->redPiecesOnBar > 0)
	{
		Vector2D pos = Vector2D(kBackgammonBoardWidth / 2 - 17, 200);
		mRedChecker->Render(pos, SDL_FLIP_NONE, 0.0);

		SDL_Rect destRect = SDL_Rect();
		destRect.x = kBackgammonBoardWidth / 2 - 15;
		destRect.y = 200;
		destRect.h = 30;
		destRect.w = 30;

		SDL_Rect originRect = SDL_Rect();
		originRect.x = 0;
		originRect.y = 26 * (mBackgammonBoard->redPiecesOnBar - 1);
		originRect.h = 26;
		originRect.w = 30;

		mNumberSpriteSheet->Render(originRect, destRect);
	}
	if (mBackgammonBoard->whitePiecesOnBar > 0)
	{
		Vector2D pos = Vector2D(kBackgammonBoardWidth / 2 - 17, 300);
		mWhiteChecker->Render(pos, SDL_FLIP_NONE, 0.0);

		SDL_Rect destRect = SDL_Rect();
		destRect.x = kBackgammonBoardWidth / 2 - 15;
		destRect.y = 300;
		destRect.h = 30;
		destRect.w = 30;

		SDL_Rect originRect = SDL_Rect();
		originRect.x = 0;
		originRect.y = 26 * (mBackgammonBoard->whitePiecesOnBar - 1);
		originRect.h = 26;
		originRect.w = 30;

		mNumberSpriteSheet->Render(originRect, destRect);
	}

	if (mBackgammonBoard->redPiecesOffBoard > 0)
	{
		Vector2D pos = Vector2D(70, kBackgammonBoardHeight / 6 * 5.15);
		mRedChecker->Render(pos, SDL_FLIP_NONE, 0.0);

		SDL_Rect destRect = SDL_Rect();
		destRect.x = 70;
		destRect.y = kBackgammonBoardHeight / 6 * 5.15;
		destRect.h = 30;
		destRect.w = 30;

		SDL_Rect originRect = SDL_Rect();
		originRect.x = 0;
		originRect.y = 26 * (mBackgammonBoard->redPiecesOffBoard - 1);
		originRect.h = 26;
		originRect.w = 30;

		mNumberSpriteSheet->Render(originRect, destRect);

	}
	if (mBackgammonBoard->whitePiecesOffBoard > 0)
	{
		Vector2D pos = Vector2D(180, kBackgammonBoardHeight / 6 * 5.15);
		mWhiteChecker->Render(pos, SDL_FLIP_NONE, 0.0);

		SDL_Rect destRect = SDL_Rect();
		destRect.x = 180;
		destRect.y = kBackgammonBoardHeight / 6 * 5.15;
		destRect.h = 30;
		destRect.w = 30;

		SDL_Rect originRect = SDL_Rect();
		originRect.x = 0;
		originRect.y = 26 * (mBackgammonBoard->whitePiecesOffBoard - 1);
		originRect.h = 26;
		originRect.w = 30;

		mNumberSpriteSheet->Render(originRect, destRect);
	}

	for each (BackgammonMove move in mPlayers[mPlayerTurn]->mSelectedAvailableMoves)
	{
		if (move.pointTo == 25)
		{
			Vector2D pos = Vector2D(15, kBackgammonBoardHeight / 6 * 4.65);
			mBearOffHighlight->Render(pos, SDL_FLIP_NONE, 0.0);
		}
	}
	

	//SDL_RenderDrawRects(mRenderer, boardCollisionBoxes, 26);
}

