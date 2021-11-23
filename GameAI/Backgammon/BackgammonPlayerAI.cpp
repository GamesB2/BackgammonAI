#include "BackgammonPlayerAI.h"
#include <iostream>
#include <chrono>
#include <SDL.h>
#include "../Texture2D.h"

using namespace::std;

//Single checker on a point is known as blot, this is a bad thing
const int kBlotWeight = 50;
//Two or more checkers on a point is known as a point, we want these
const int kPointWeight = 30;
//Six points in a row creates a prime, meaning the opponent cannot pass it at all, this is very good
const int kPrimeWeight = 150;
//Having a piece on the bar is bad for that player as it throttles their available moves and makes it harder to win
const int kPieceOnBarWeight = 100;
//If a player has a prime on the opponents home board while they have a piece on the bar, they have no moves, big want.
const int kClosedOutWeight = 250;
//All of our pieces must be removed to win, we obviously want this
const int kPiecesOffBoardWeight = 200;
const int kPiecesCanBearOffWeight = 100;
//We want to be on their homeboard ideally to create better points and eventually bearoff;
const int kPieceOnOpponentHomeBoardWeight = 200;

const int U = 600;
const int L = -600;
//--------------------------------------------------------------------------------------------------

BackgammonPlayerAI::BackgammonPlayerAI(SDL_Renderer* renderer, BACKGAMMONCOLOUR colour, BackgammonBoard* board, SDL_Rect collisions[25])
	: BackgammonPlayer(renderer, colour, board, collisions)
{
	//Start values.
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

BackgammonPlayerAI::~BackgammonPlayerAI()
{
	mBackgammonBoard = NULL;

	mHighlightPositions = NULL;

	delete mSelectAPieceTexture;
	mSelectAPieceTexture = NULL;

	delete mSelectedPiecePosition;
	mSelectedPiecePosition = NULL;
}

//--------------------------------------------------------------------------------------------------

void BackgammonPlayerAI::DealWithTurn(SDL_Event e)
{
	//Depending on AI selected

	cout << "Thinking..." << endl;
	boardsCreated = 0;

	auto start = std::chrono::high_resolution_clock::now();

	switch (mCurrentAI)
	{
	case EXPECTIMAX:
		ExpectiMiniMax(*mBackgammonBoard, kExpectimaxSearchDepth);
		break;
	case STARMINIMAX:
		Star2Maximise(*mBackgammonBoard, kExpectimaxSearchDepth, -MAXINT, MAXINT);
		break;
	case RANDOM:
		Random(*mBackgammonBoard);
	}

	auto finish = std::chrono::high_resolution_clock::now();

	//"Refactoring"

	if (mBackgammonBoard->movesAvailable == 4)
	{
		if (mBestMovePair.first.type != NONEAVAILABLE)
		{
			//cout << " moved " << mBestMovePair.first.pointFrom << " to " << mBestMovePair.first.pointTo << endl;

			if (mBestMovePair.second.type != NONEAVAILABLE)
			{
				//cout << ColourAsString[(int)mTeamColour] << " moved " << mBestMovePair.second.pointFrom << " to " << mBestMovePair.second.pointTo << endl;

				if (mBestMovePair.third.type != NONEAVAILABLE)
				{
					//cout << ColourAsString[(int)mTeamColour] << " moved " << mBestMovePair.third.pointFrom << " to " << mBestMovePair.third.pointTo << endl;
					
					if (mBestMovePair.first.type != NONEAVAILABLE)
					{
						//cout << ColourAsString[(int)mTeamColour] << " moved " << mBestMovePair.fourth.pointFrom << " to " << mBestMovePair.fourth.pointTo << endl;
					}
				}
			}
		}
	}
	else
	{
		if (mBestMovePair.first.type != NONEAVAILABLE)
		{
			cout << "AI moved " << mBestMovePair.first.pointFrom << " to " << mBestMovePair.first.pointTo << endl;

			if (mBestMovePair.second.type != NONEAVAILABLE)
			{
				cout << "AI moved " << mBestMovePair.second.pointFrom << " to " << mBestMovePair.second.pointTo << endl;
			}
		}
	}

	UseMovePairOnBoard(mBestMovePair, mBackgammonBoard);

	mBackgammonBoard->movesAvailable = 0;

	std::chrono::duration<double> elapsed = finish - start;

	cout << "Boards created during search: " << boardsCreated << endl;
	cout << "Elapsed time :" << elapsed.count() << endl;
}

void BackgammonPlayerAI::Random(BackgammonBoard board)
{
	vector<BackgammonMovePair> vecPairs;
	GetAllMovePairs(board, mTeamColour, &vecPairs);

	int randChosen = rand() % vecPairs.size();

	mBestMovePair = vecPairs[randChosen];

	if (debug) { cout << "Moveset " << randChosen << " chosen of " << vecPairs.size() << " possible movesets" << endl; }

}

int BackgammonPlayerAI::ExpectiMiniMax(BackgammonBoard board, int depth)
{
	//Maximise returns highest scored board
	//Minimise returns lowest scored board
	//Chance returns average of all subsequent boards divided by number of dice rolls (21)

	return Maximise(board, depth);
}

//--------------------------------------------------------------------------------------------------

int BackgammonPlayerAI::Maximise(BackgammonBoard board, int depth)
{
	if (depth == 0)
	{
		int score = ScoreTheBoard(board);
		if (debug) { cout << "Terminal Board scored at: " << score << endl; }
		return score;
	}

	int score = 0;
	int max = -MaxInt;

	vector<BackgammonMovePair> movePairs;

	GetAllMovePairs(board, mTeamColour, &movePairs);

	if (movePairs.size() == 0 &&(board.redPiecesOffBoard == 15 || board.whitePiecesOffBoard == 15))
	{
		return max;
	}
	else if (movePairs.size() == 0)
	{
		score = Chance(board, depth, MIN);
	}

	for (int i = 0; i < movePairs.size(); i++) 
	{
		BackgammonBoard * newBoard = new BackgammonBoard(board);
		boardsCreated++;
		UseMovePairOnBoard(movePairs[i], newBoard);
		score = Chance(*newBoard, depth, MIN);
		delete newBoard;
		if (score > max)
		{
			max = score;
			if (depth == kExpectimaxSearchDepth)
			{
				mBestMovePair = movePairs[i];
			}
		}
	}
	return max;
}

int BackgammonPlayerAI::Chance(BackgammonBoard board, int depth, MinMaxNext next)
{
	int score = 0;

	for (int i = 1; i < 7; i++)
	{
		for (int j = i; j < 7; j++)
		{
			BackgammonBoard * newboard = new BackgammonBoard(board);
			boardsCreated++;
			if (i == j)
			{
				newboard->diceAvailable[0] = i;
				newboard->diceAvailable[1] = i;
				newboard->diceAvailable[2] = i;
				newboard->diceAvailable[3] = i;
				newboard->movesAvailable = 4;
			}
			else
			{
				newboard->diceAvailable[0] = i;
				newboard->diceAvailable[1] = j;
				newboard->movesAvailable = 2;
			}
			if (next == MAX)
			{
				//cout << "Chance node -> Board dice: " << i << ", " << j << endl;
				score += Maximise(*newboard, depth - 1);
			}
			if (next == MIN)
			{
				score += Minimise(*newboard, depth - 1);
			}
			delete newboard;
		}
	}
	if (debug) { cout << "Chance node scored at : " << score / 21 << endl; }
	return score / 21;
}

int BackgammonPlayerAI::Minimise(BackgammonBoard board, int depth)
{
	if (depth == 0)
	{
		int score = ScoreTheBoard(board);
		//cout << "Board scored at: " << score << endl;
		return score;
	}

	int score = 0;
	int min = MaxInt;
	
	vector<BackgammonMovePair> movePairs;

	GetAllMovePairs(board, mEnemyColour, &movePairs);

	if (movePairs.size() == 0 && (board.redPiecesOffBoard == 15 || board.whitePiecesOffBoard == 15))
	{
		return min;
	}
	else if (movePairs.size() == 0)
	{
		score = Chance(board, depth, MIN);
	}

	for (int i = 0; i < movePairs.size(); i++)
	{
		BackgammonBoard* newBoard = new BackgammonBoard(board);
		boardsCreated++;
		UseMovePairOnBoard(movePairs[i], newBoard);
		score = Chance(*newBoard, depth, MAX);
		delete newBoard;
		if (score < min)
		{
			min = score;
		}
	}

	return min;
}

//--------------------------------------------------------------------------------------------------

int BackgammonPlayerAI::Star2Maximise(BackgammonBoard board, int depth, int alpha, int beta)
{
	if (depth <= 0)
	{
		int score = ScoreTheBoard(board);
		if (debug) { cout << "Terminal Board scored at: " << score << endl; }
		return score;
	}

	int score = 0;
	int max = L;
	
	vector<BackgammonMovePair> movePairs;

	GetAllMovePairs(board, mTeamColour, &movePairs);

	if (movePairs.size() == 0 && (board.redPiecesOffBoard == 15 || board.whitePiecesOffBoard == 15))
	{
		return ScoreTheBoard(board);
	}
	else if (movePairs.size() == 0)
	{
		score = Star2Chance(board, depth, max, beta, MIN);
	}

	for (int i = 0; i < movePairs.size(); i++)
	{
		BackgammonBoard * newBoard = new BackgammonBoard(board);
		boardsCreated++;
		UseMovePairOnBoard(movePairs[i], newBoard);
		score = Star2Chance(*newBoard, depth, max, beta, MIN);
		delete newBoard;
		if (score > max)
		{
			max = score;
			if (depth == kExpectimaxSearchDepth)
			{
				mBestMovePair = movePairs[i];
			}
		}
	}
	return max;
}

int BackgammonPlayerAI::Star2Chance(BackgammonBoard board, int depth, int alpha, int beta, MinMaxNext next)
{
	//local A, B, i, v, vsum, AX, BX, s[l, w[];
	if (depth <= 0)
	{
		int score = ScoreTheBoard(board);
		//cout << "Board scored at: " << score << endl;
		return score;
	}

	int N = 21;
	int probedBounds[21];
	int count = 0;
	
	int vsum = 0;

	int A = N * (alpha - U);
	int B = N * (beta - L);

	if (next == MAX)
	{
		int AX = max(A, L);

		for (int i = 1; i < 7; i++)
		{
			for (int j = i; j < 7; j++)
			{
				B = B - L;
				int BX = min(B, U);
				probedBounds[count] = Probe(board, depth - 1, AX, BX, i, j, next);
				if (probedBounds[count] >= B)
				{
					if (debug) { cout << "Tree pruned due probe " << probedBounds[count] << " > " << B << endl; }
					return(beta);
				}

				B = B - probedBounds[count];
			}
		}
		
		count = 0;

		for (int i = 1; i < 7; i++)
		{
			for (int j = i; j < 7; j++)
			{
				A = A - U;
				int BX = min(B, U);
				AX = max(A, L);

				B = B - probedBounds[count];

				BackgammonBoard * newBoard = new BackgammonBoard(board);
				boardsCreated++;
				if (i == j)
				{
					newBoard->movesAvailable = 4;
					newBoard->diceAvailable[0] = i;
					newBoard->diceAvailable[1] = i;
					newBoard->diceAvailable[2] = i;
					newBoard->diceAvailable[3] = i;
				}
				else
				{
					newBoard->movesAvailable = 2;
					newBoard->diceAvailable[0] = i;
					newBoard->diceAvailable[1] = j;
				}

				int v = Star2Maximise(*newBoard, depth - 1, AX, BX);
				if (v <= A)
				{
					if (debug) { cout << "Tree pruned during extensive search" << endl; }
					return(alpha);
				}
				if (v >= B)
				{
					if (debug) { cout << "Tree pruned during extensive search" << endl; }
					return(beta);
				}
				vsum += v;
				A = A - v;
				B = B - v;

				delete newBoard;
			}
		}
	}

	if (next == MIN)
	{

		int BX = min(B, U);

		for (int i = 1; i <= 7; i++)
		{
			for (int j = i; j < 7; j++)
			{
				A = A + U;
				int AX = max(A, L);
				probedBounds[count] = Probe(board, depth - 1, AX, BX, i, j, next);
				if (probedBounds[count] <= A)
				{
					if (debug) { cout << "Tree pruned due to probe " << probedBounds[count] << " < " << A << endl; }
					return(alpha);
				}

				A = A - probedBounds[count];
				count++;
			}
		}

		count = 0;

		for (int i = 1; i <= 7; i++)
		{
			for (int j = i; j < 7; j++)
			{
				B = B - L;
				int AX = max(A, L);
				BX = min(B, U);

				A = A + probedBounds[count];

				BackgammonBoard * newBoard = new BackgammonBoard(board);
				boardsCreated++;
				if (i == j)
				{
					newBoard->movesAvailable = 4;
					newBoard->diceAvailable[0] = i;
					newBoard->diceAvailable[1] = i;
					newBoard->diceAvailable[2] = i;
					newBoard->diceAvailable[3] = i;
				}
				else
				{
					newBoard->movesAvailable = 2;
					newBoard->diceAvailable[0] = i;
					newBoard->diceAvailable[1] = j;
				}


				int v = Star2Minimise(*newBoard, depth - 1, AX, BX);
				if (v <= A)
				{
					if (debug) { cout << "Tree pruned during extensive search" << endl; }
					return(alpha);
				}
				if (v >= B)
				{
					if (debug) { cout << "Tree pruned during extensive search" << endl; }
					return(beta);
				}
				vsum += v;
				A = A - v;
				B = B - v;
				count++;
				delete newBoard;
			}
		}
	}

	return(vsum / N);
}

int BackgammonPlayerAI::Star2Minimise(BackgammonBoard board, int depth, int alpha, int beta)
{
	if (depth <= 0)
	{
		int score = ScoreTheBoard(board);
		//cout << "Board scored at: " << score << endl;
		return score;
	}

	int score = 0;
	int min = U;

	vector<BackgammonMovePair> movePairs;

	GetAllMovePairs(board, mEnemyColour, &movePairs);

	if (movePairs.size() == 0 && (board.redPiecesOffBoard == 15 || board.whitePiecesOffBoard == 15))
	{
		return min;
	}
	else if (movePairs.size() == 0)
	{
		score = Star2Chance(board, depth, alpha, min,  MIN);
	}

	for (int i = 0; i < movePairs.size(); i++)
	{
		BackgammonBoard* newBoard = new BackgammonBoard(board);
		boardsCreated++;
		UseMovePairOnBoard(movePairs[i], newBoard);
		score = Star2Chance(*newBoard, depth, alpha, min, MAX);
		delete newBoard;
		if (score < min)
		{
			min = score;
		}
	}

	return min;
}

int BackgammonPlayerAI::Probe(BackgammonBoard board, int depth, float alpha, float beta, int i, int j, MinMaxNext next)
{

	if (depth <= 0)
	{
		if (next == MAX)
		{
			return Star2Maximise(board, depth, alpha, beta);
		}
		if (next == MIN)
		{
			return Star2Minimise(board, depth, alpha, beta);
		}
	}


	board.diceAvailable[0] = i;
	board.diceAvailable[1] = j;
	if (i == j)
	{
		board.diceAvailable[2] = i;
		board.diceAvailable[3] = j;
		board.movesAvailable = 4;
	}
	else
	{
		board.movesAvailable = 2;
	}


	vector<BackgammonMovePair> vecPairs;

	if (next == MAX)
	{
		GetAllMovePairs(board, mTeamColour, &vecPairs);
		for each (BackgammonMovePair pair in vecPairs)
		{
			if (pair.first.type == HIT || pair.second.type == HIT)
			{
				UseMovePairOnBoard(pair, &board);
				return Star2Chance(board, depth, alpha, beta,  MIN);
				/*int score = ScoreTheBoard(board);
				if (debug) { cout << "Probe scores board at " << score << endl; }
				return ScoreTheBoard(board);*/
			}
		}
		for each (BackgammonMovePair pair in vecPairs)
		{
			if (pair.first.checkerTo == 25 || pair.second.checkerTo == 25)
			{
				UseMovePairOnBoard(pair, &board);
				return Star2Chance(board, depth, alpha, beta, MIN);
				/*int score = ScoreTheBoard(board);
				if (debug) { cout << "Probe scores board at " << score << endl; }
				return ScoreTheBoard(board);*/
			}
		}

		for each (BackgammonMovePair pair in vecPairs)
		{
			if (pair.first.checkerTo >= 1 || pair.second.checkerTo >= 1)
			{
				UseMovePairOnBoard(pair, &board);
				return Star2Chance(board, depth, alpha, beta, MIN);
				/*int score = ScoreTheBoard(board);
				if (debug) { cout << "Probe scores board at " << score << endl; }
				return ScoreTheBoard(board);*/
			}
		}

		if (vecPairs.size() != 0)
		{
			int randchosen = rand() % vecPairs.size();
			UseMovePairOnBoard(vecPairs[randchosen], &board);
			return Star2Chance(board, depth, alpha, beta, MIN);
			/*int score = ScoreTheBoard(board);
			if (debug) { cout << "Probe scores board at " << score << endl; }
			return ScoreTheBoard(board);*/
		}
		
		if (next == MAX)
		{
			return Star2Maximise(board, depth, alpha, beta);
		}
		if (next == MIN)
		{
			return Star2Minimise(board, depth, alpha, beta);
		}
	}
	else if (next == MIN)
	{
		GetAllMovePairs(board, mEnemyColour, &vecPairs);
		for each (BackgammonMovePair pair in vecPairs)
		{
			if (pair.first.type == HIT || pair.second.type == HIT)
			{
				UseMovePairOnBoard(pair, &board);
				return Star2Chance(board, depth, alpha, beta,  MAX);
				/*int score = ScoreTheBoard(board);
				if (debug) { cout << "Probe scores board at " << score << endl; }
				return ScoreTheBoard(board);*/
			}
		}

		for each (BackgammonMovePair pair in vecPairs)
		{
			if (pair.first.checkerTo == 25 || pair.second.checkerTo == 25)
			{
				UseMovePairOnBoard(pair, &board);
				return Star2Chance(board, depth, alpha, beta, MAX);
				/*int score = ScoreTheBoard(board);
				if (debug) { cout << "Probe scores board at " << score << endl; }
				return ScoreTheBoard(board);*/
			}
		}

		for each (BackgammonMovePair pair in vecPairs)
		{
			if (pair.first.checkerTo >= 1 || pair.second.checkerTo >= 1)
			{
				UseMovePairOnBoard(pair, &board);
				return Star2Chance(board, depth, alpha, beta, MAX);
				/*int score = ScoreTheBoard(board);
				if (debug) { cout << "Probe scores board at " << score << endl; }
				return ScoreTheBoard(board);*/
			}
		}
		if (vecPairs.size() != 0)
		{
			int randchosen = rand() % vecPairs.size();
			UseMovePairOnBoard(vecPairs[randchosen], &board);
			return Star2Chance(board, depth, alpha, beta, MAX);
			/*int score = ScoreTheBoard(board);
			if (debug) { cout << "Probe scores board at " << score << endl; }
			return ScoreTheBoard(board);*/
		}
		int score = ScoreTheBoard(board);
		if (debug) { cout << "Probe scores board at " << score << endl; }
		return ScoreTheBoard(board);
	}
}

int BackgammonPlayerAI::ScoreTheBoard(BackgammonBoard board)
{
	int score = 0;

	int opponentBlots = 0;
	int friendlyBlots = 0;

	int opponentPoints = 0;
	int	friendlyPoints = 0;

	int friendlyPointCount = 0;
	int opponentPointCount = 0;

	for (int i = 1; i < 25; i++)
	{
		if (board.currentLayout[i][0] == mTeamColour && board.currentLayout[i][1] == mTeamColour)
		{
			friendlyPoints++;
			friendlyPointCount++;
			opponentPointCount = 0;
		}
		if (board.currentLayout[i][0] == mEnemyColour && board.currentLayout[i][1] == mEnemyColour)
		{
			opponentPoints++;
			opponentPointCount++;
			friendlyPointCount = 0;
		}
		if (board.currentLayout[i][0] == mTeamColour && board.currentLayout[i][1] == NONE)
		{
			friendlyBlots++;
			friendlyPointCount = 0;
			opponentPointCount = 0;
		}
		if (board.currentLayout[i][0] == mTeamColour && board.currentLayout[i][1] == NONE)
		{
			opponentBlots++;
			friendlyPointCount = 0;
			opponentPointCount = 0;
		}
		if (friendlyPointCount == 6)
		{
			score += kPrimeWeight;
		}
		if (opponentPointCount == 6)
		{
			score -= kPrimeWeight;
		}
	}

	score += opponentBlots * kBlotWeight;
	score -= friendlyBlots * kBlotWeight;
	score += friendlyPoints * kPointWeight;
	score -= opponentPoints * kPointWeight;

	if (mTeamColour == RED)
	{
		score += board.whitePiecesOnBar * kPieceOnBarWeight;
		score -= board.redPiecesOnBar * kPieceOnBarWeight;
		score += board.redPiecesOffBoard * kPiecesOffBoardWeight;
		score -= board.whitePiecesOffBoard * kPiecesOffBoardWeight;

		if (board.redBearOff)
		{
			score += kPiecesCanBearOffWeight;
		}
		if (board.whiteBearOff)
		{
			score -= kPiecesCanBearOffWeight;
		}
	}
	if (mTeamColour == WHITE)
	{
		score -= board.whitePiecesOnBar * kPieceOnBarWeight;
		score += board.redPiecesOnBar * kPieceOnBarWeight;
		score -= board.redPiecesOffBoard * kPiecesOffBoardWeight;
		score += board.whitePiecesOffBoard * kPiecesOffBoardWeight;
		if (board.redBearOff)
		{
			score -= kPiecesCanBearOffWeight;
		}
		if (board.whiteBearOff)
		{
			score += kPiecesCanBearOffWeight;
		}
	}

	for (int i = 1; i < 25; i++)
	{
		if (board.currentLayout[i][0] == RED && mTeamColour == RED)
		{
			score += i * 2;
		}
		if (board.currentLayout[i][0] == WHITE && mTeamColour == RED)
		{
			score -= (24 - i) * 2;
		}
		if (board.currentLayout[i][0] == WHITE && mTeamColour == WHITE)
		{
			score += (24 - i) * 2;
		}
		if (board.currentLayout[i][0] == RED && mTeamColour == WHITE)
		{
			score -= i * 2;
		}
	}

	return score;
}

bool BackgammonPlayerAI::DiceRoll(SDL_Event e)
{
	return true;
}

void BackgammonPlayerAI::UseMovePairOnBoard(BackgammonMovePair pair, BackgammonBoard * board)
{
	if (pair.numberOfMoves == 2)
	{
		MakeAMove(board, &pair.first);
		if (pair.second.type == NONEAVAILABLE)
		{
			return;
		}
		MakeAMove(board, &pair.second);
		return;
	}
	else if (pair.numberOfMoves == 4)
	{
		MakeAMove(board, &pair.first);
		if (pair.second.type == NONEAVAILABLE)
		{
			return;
		}
		MakeAMove(board, &pair.second);
		if (pair.third.type == NONEAVAILABLE)
		{
			return;
		}
		MakeAMove(board, &pair.third);
		if (pair.fourth.type == NONEAVAILABLE)
		{
			return;
		}
		MakeAMove(board, &pair.fourth);
	}
}

void BackgammonPlayerAI::GetAllMovePairs(BackgammonBoard board, BACKGAMMONCOLOUR mTeamColour, vector<BackgammonMovePair> * vecPairs)
{
	vector<BackgammonMove> currentMoves;

	vecPairs->clear();

	GetAllMoveOptions(board, mTeamColour, &currentMoves);

	if (currentMoves.size() == 0)
	{
		return;
	}

	if (board.movesAvailable == 2)
	{
		for (int move1 = 0; move1 < currentMoves.size(); move1++)
		{
			BackgammonBoard * nextBoard = new BackgammonBoard(board);
			MakeAMove(nextBoard, &currentMoves[move1]);
			vector<BackgammonMove> secondMoves;
			GetAllMoveOptions(*nextBoard, mTeamColour, &secondMoves);

			if (secondMoves.size() == 0)
			{
				BackgammonMovePair pair;
				pair.first = currentMoves[move1];
				pair.second.type = NONEAVAILABLE;
				pair.numberOfMoves = 2;
				vecPairs->push_back(pair);
				continue;
			}
			for (int move2 = 0; move2 < secondMoves.size(); move2++)
			{
				BackgammonMovePair pair;
				pair.first = currentMoves[move1];
				pair.second = secondMoves[move2];
				pair.numberOfMoves = 2;
				vecPairs->push_back(pair);
			}
			delete nextBoard;
		}
	}

	else if (board.movesAvailable == 4)
	{
		for (int move1 = 0; move1 < currentMoves.size(); move1++)
		{
			BackgammonBoard * nextBoard = new BackgammonBoard(board);
			MakeAMove(nextBoard, &currentMoves[move1]);
			vector<BackgammonMove> secondMoves;
			GetAllMoveOptions(*nextBoard, mTeamColour, &secondMoves);

			if (secondMoves.size() == 0)
			{
				BackgammonMovePair pair;
				pair.first = currentMoves[move1];
				pair.second.type = NONEAVAILABLE;
				pair.numberOfMoves = 4;
				vecPairs->push_back(pair);
				continue;
			}

			for (int move2 = 0; move2 < secondMoves.size(); move2++)
			{
				BackgammonBoard * nextBoard2 = new BackgammonBoard(*nextBoard);
				MakeAMove(nextBoard2, &secondMoves[move2]);
				vector<BackgammonMove> thirdMoves;
				GetAllMoveOptions(*nextBoard2, mTeamColour, &thirdMoves);

				if (thirdMoves.size() == 0)
				{
					BackgammonMovePair pair;
					pair.first = currentMoves[move1];
					pair.second = secondMoves[move2];
					pair.third.type = NONEAVAILABLE;
					pair.numberOfMoves = 4;
					vecPairs->push_back(pair);
					continue;
				}

				for (int move3 = 0; move3 < thirdMoves.size(); move3++)
				{
					BackgammonBoard * nextBoard3 = new BackgammonBoard(*nextBoard2);
					MakeAMove(nextBoard3, &thirdMoves[move3]);
					vector<BackgammonMove> fourthMoves;
					GetAllMoveOptions(*nextBoard3, mTeamColour, &fourthMoves);

					if (fourthMoves.size() == 0)
					{
						BackgammonMovePair pair;
						pair.first = currentMoves[move1];
						pair.second = secondMoves[move2];
						pair.third = thirdMoves[move3];
						pair.fourth.type = NONEAVAILABLE;
						pair.numberOfMoves = 4;
						vecPairs->push_back(pair);
						continue;
					}
					for (int move4 = 0; move4 < fourthMoves.size(); move4++)
					{
						BackgammonMovePair pair;
						pair.first = currentMoves[move1];
						pair.second = secondMoves[move2];
						pair.third = thirdMoves[move3];
						pair.fourth = fourthMoves[move4];
						pair.numberOfMoves = 4;
						vecPairs->push_back(pair);
					}
					delete nextBoard3;
				}
				delete nextBoard2;
			}
			delete nextBoard;
		}
	}
}

void BackgammonPlayerAI::SetAI(int choice)
{
	switch (choice)
	{
	case 1:
		mCurrentAI = EXPECTIMAX;
		cout << "AI set to Expectimax" << endl;
		break;
	case 2:
		mCurrentAI = STARMINIMAX;
		cout << "AI set to Star Minimax" << endl;
		break;
	case 3:
		mCurrentAI = RANDOM;
		cout << "AI set to random choice" << endl;
		break;
	}
}
//--------------------------------------------------------------------------------------------------
