/*
 * EngineState.h partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 09/11/2013
 */
#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#include "Engine.h"

class EngineState;

// Base Class EngineStateListener handles state trastitions in a stack-based interface
class EngineStateListener
{
public:
	EngineStateListener(){};
	virtual ~EngineStateListener(){};

	virtual void manageEngineState(Ogre::String stateName, EngineState* state) = 0;
	virtual EngineState* findByName(Ogre::String stateName) = 0;
	virtual void changeState(EngineState *state) = 0;
	virtual bool pushState(EngineState* state) = 0;
	virtual void popState() = 0;
	virtual void pauseState() = 0;
	virtual void shutdown() = 0;
	virtual void popAllAndPushNewEngineState(EngineState* state) = 0;
};

// Extended EngineState class inherits from OIS and Ogre Listeners
class EngineState : public OIS::KeyListener, public OIS::MouseListener, public OgreBites::SdkTrayListener
{
public:
	static void create(EngineStateListener* parentState, const Ogre::String name){};
	void destroy() { delete this; }
	
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause(){ return true; }
	virtual void resume(){};
	virtual void update(double timeSinceLastFrame) = 0;

protected:
	EngineState(){};

	EngineState* findByName(Ogre::String stateName){ return mParentName->findByName(stateName); }
	void changeState(EngineState* state){ mParentName->changeState(state); }
	bool pushState(EngineState* state){ return mParentName->pushState(state); }
	void popState(){ mParentName->popState(); }
	void shutdown(){ mParentName->shutdown(); }
	void popAllAndPushNewEngineState(EngineState* state){ mParentName->popAllAndPushNewEngineState(state); }

	EngineStateListener* mParentName;
	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode* mCameraNode;
	Ogre::FrameEvent mFrameEvent;
};

// Define the EngineState Class 
#define DECLARE_ENGINESTATE_CLASS(T)									\
static void create(EngineStateListener* parent, const Ogre::String name)\
{																		\
	T* engineState = new T();											\
	engineState->mParentName = parent;									\
	parent->manageEngineState(name, engineState);						\
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif