#ifndef GAME_H
#define GAME_H

#pragma once

#include "Engine.h"
#include "EngineStateManager.h"

class Game
{
public:
	Game();
	~Game();

	void start();
private:
	EngineStateManager* mEngineStateManager;
};

#endif