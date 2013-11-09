/*
 * EngineStateManager.cpp partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 09/11/2013
 */
#include "EngineStateManager.h"
#include <OgreWindowEventUtilities.h>

EngineStateManager::EngineStateManager()
{
	mShutdown = false;
}

EngineStateManager::~EngineStateManager()
{
	state_info info;

	while(!mStateStack.empty())
	{
		mStateStack.back()->exit();
		mStateStack.pop_back();
	}

	while(!mStatesInfo.empty())
	{
		info = mStatesInfo.back();
		info.state->destroy();
		mStatesInfo.pop_back();
	}
}

EngineState* EngineStateManager::findByName(Ogre::String stateName)
{
	std::vector<state_info>::iterator i;

	for(i=mStatesInfo.begin(); i!=mStatesInfo.end(); i++)
	{
		if(i->name==stateName)
			return i->state;
	}

	return 0;
}

void EngineStateManager::manageEngineState(Ogre::String stateName, EngineState* state)
{
	try
	{
		state_info newStateInfo;
		newStateInfo.name = stateName;
		newStateInfo.state = state;
		mStatesInfo.push_back(newStateInfo);
	}
	catch(std::exception& e)
	{
		delete state;
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR, "Cannot manage new EngineState\n" + Ogre::String(e.what()), "EngineStateManager.cpp (33)");
	}
}

void EngineStateManager::start(EngineState* state)
{
	changeState(state);

	int timeSinceLastFrame = 1;
	int startTime = 0;

	while(!mShutdown)
	{
		if(Engine::getSingletonPtr()->mRenderWnd->isClosed())mShutdown = true;

		Ogre::WindowEventUtilities::messagePump();

		if(Engine::getSingletonPtr()->mRenderWnd->isActive())
		{
			startTime = Engine::getSingletonPtr()->mTimer->getMillisecondsCPU();

			Engine::getSingletonPtr()->mKeyboard->capture();
			Engine::getSingletonPtr()->mMouse->capture();

			mStateStack.back()->update(timeSinceLastFrame);

			Engine::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			Engine::getSingletonPtr()->mRoot->renderOneFrame();

			timeSinceLastFrame = Engine::getSingletonPtr()->mTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			Sleep(1000);
#else
			sleep(1);
#endif
		}
	}

	Engine::getSingletonPtr()->mLog->logMessage("Main loop exit");
}


void EngineStateManager::changeState(EngineState* state)
{
	if(!mStateStack.empty())
	{
		mStateStack.back()->exit();
		mStateStack.pop_back();
	}

	mStateStack.push_back(state);
	init(state);
	mStateStack.back()->enter();
}

bool EngineStateManager::pushState(EngineState* state)
{
	if(!mStateStack.empty())
	{
		if(!mStateStack.back()->pause())
			return false;
	}

	mStateStack.push_back(state);
	init(state);
	mStateStack.back()->enter();

	return true;
}

void EngineStateManager::popState()
{
	if(!mStateStack.empty())
	{
		mStateStack.back()->exit();
		mStateStack.pop_back();
	}

	if(!mStateStack.empty())
	{
		init(mStateStack.back());
		mStateStack.back()->resume();
	}
	else
		shutdown();
}

void EngineStateManager::popAllAndPushNewEngineState(EngineState* state)
{
	while(!mStateStack.empty())
	{
		mStateStack.back()->exit();
		mStateStack.pop_back();
	}

	pushState(state);
}

void EngineStateManager::pauseState()
{
	if(!mStateStack.empty())
	{
		mStateStack.back()->pause();
	}

	if(mStateStack.size() > 2)
	{
		init(mStateStack.at(mStateStack.size() - 2));
		mStateStack.at(mStateStack.size() - 2)->resume();
	}
}

void EngineStateManager::shutdown()
{
	mShutdown = true;
}

void EngineStateManager::init(EngineState* state)
{
	Engine::getSingletonPtr()->mKeyboard->setEventCallback(state);
	Engine::getSingletonPtr()->mMouse->setEventCallback(state);
	Engine::getSingletonPtr()->mTrayMgr->setListener(state);

	Engine::getSingletonPtr()->mRenderWnd->resetStatistics();
}
