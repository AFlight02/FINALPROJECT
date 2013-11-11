/*
 * GameState.h partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 10/11/2013
 */
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#pragma once

#include "EngineState.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

class GameState : public EngineState
{
public:
	GameState();
	DECLARE_ENGINESTATE_CLASS(GameState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();

	void moveCamera();
};

#endif
