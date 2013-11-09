/*
 * EngineStateManager.h partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 09/11/2013
 */
#ifndef ENGINE_STATE_MANAGER_H
#define ENGINE_STATE_MANAGER_H

#pragma once

#include "EngineState.h"

class EngineStateManager : public EngineStateListener
{
public:
	// Create a state_info struct type including a state and a name
	typedef struct
	{
		Ogre::String name;
		EngineState* state;
	} state_info;

	EngineStateManager();
	~EngineStateManager();

	EngineState* findByName(Ogre::String stateName);
	void manageEngineState(Ogre::String stateName, EngineState* state);
	void start(EngineState* state);
	void changeState(EngineState* state);
	bool pushState(EngineState* state);
	void popState();
	void pauseState();
	void shutdown();
	void popAllAndPushNewEngineState(EngineState* state);

protected:
	void init(EngineState *state);
	std::vector<EngineState*> mStateStack;
	std::vector<state_info> mStatesInfo;
	bool mShutdown;
};

#endif