//------------------------------------------------------------------------
//  Author: Paul Roberts 2015
//------------------------------------------------------------------------

#include "GameScreenManager.h"
#include "GameScreen.h"
#include "Backgammon\GameScreen_Backgammon.h"


//--------------------------------------------------------------------------------------------------

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	mRenderer = renderer;
	mCurrentScreen = NULL;

	//Ensure the first screen is set up.
	ChangeScreen(startScreen);
}

//--------------------------------------------------------------------------------------------------

GameScreenManager::~GameScreenManager()
{
	mRenderer = NULL;

	delete mCurrentScreen;
	mCurrentScreen = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::Render()
{
	mCurrentScreen->Render();
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::Update(size_t deltaTime, SDL_Event e)
{
	mCurrentScreen->Update(deltaTime, e);
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	//Clear up the old screen.
	if(mCurrentScreen != NULL)
	{
		delete mCurrentScreen;
	}

	GameScreen_Backgammon*		tempBackgammonScreen;

	//Initialise the new screen.
	mCurrentScreenID = newScreen;
	switch(newScreen)
	{
		case SCREEN_INTRO:
		break;

		case SCREEN_MENU:
		break;

		case SCREEN_BACKGAMMON:
			tempBackgammonScreen = new GameScreen_Backgammon(mRenderer);
			mCurrentScreen = (GameScreen*)tempBackgammonScreen;
			tempBackgammonScreen = NULL;
			break;

		default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------