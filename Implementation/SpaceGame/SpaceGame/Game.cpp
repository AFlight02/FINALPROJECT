#include "Game.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"

Game::Game()
{
	mEngineStateManager = 0;
}

Game::~Game()
{
	delete mEngineStateManager;
	delete Engine::getSingletonPtr();
}

void Game::start()
{
	new Engine();
	if(!Engine::getSingletonPtr()->initOgreRenderer("SpaceGame Test", 0, 0))
		return;

	Engine::getSingletonPtr()->mLog->logMessage("SpaceGame initialized!");

	mEngineStateManager = new EngineStateManager();

	MenuState::create(mEngineStateManager, "MenuState");
	GameState::create(mEngineStateManager, "GameState");
	PauseState::create(mEngineStateManager, "PauseState");

	mEngineStateManager->start(mEngineStateManager->findByName("MenuState"));
}
