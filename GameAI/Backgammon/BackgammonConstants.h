#ifndef BACKGAMMONCONSTANTS_H
#define BACKGAMMONCONSTANTS_H

#include <iostream>
using namespace::std;

//File paths.
const string kBackgammonBoardPath			= "Images/Backgammon/BackgammonBoard2.png";
const string kBackgammonWhiteCheckerPath	= "Images/Backgammon/WhiteChecker.png";
const string kBackgammonRedCheckerPath		= "Images/Backgammon/RedChecker.png";
const string kBackgammonDiceOne				= "Images/Backgammon/1_dot.png";
const string kBackgammonDiceTwo				= "Images/Backgammon/2_dots.png";
const string kBackgammonDiceThree			= "Images/Backgammon/3_dots.png";
const string kBackgammonDiceFour			= "Images/Backgammon/4_dots.png";
const string kBackgammonDiceFive			= "Images/Backgammon/5_dots.png";
const string kBackgammonDiceSix				= "Images/Backgammon/6_dots.png";
const string kBackgammonDiceBlank			= "Images/Backgammon/0_dots.png";
const string kBackgammonRollDicebutton		= "Images/Backgammon/RollButton.png";
const string kBackgammonPieceHighlight		= "Images/Backgammon/SelectedHighlight.png";
const string kBackgammonMoveHighlight		= "Images/Backgammon/AvailableMove.png";
const string kBackgammonNumbers				= "Images/Backgammon/Numbers.png";
const string kBackgammonBearOffHighlight	= "Images/Backgammon/AvailableBearOff.png";

//Screen dimensions.
const int kBackgammonBoardWidth				= 612;		//In pixels.
const int kBackgammonBoardHeight			= 678;
const int kDiceYPos							= 536.75; //Trial and error

//Search depth in MiniMax Algorithm.
const unsigned int kExpectimaxSearchDepth				= 2;

//Number of pieces at start of game.
const unsigned int kBackgammonTotalNumberOfStartingPieces = 15;

#endif //BACKGAMMONCONSTANTS_H