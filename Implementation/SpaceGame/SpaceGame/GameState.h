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
	void getInput();
	void setupGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void itemSelected(OgreBites::SelectMenu* menu);
//	void onLeftPressed(const OIS::MouseEvent &evt);
	void update(double timeSinceLastFrame);

private:
	Ogre::SceneNode* cubeNode;
	Ogre::Entity* cubeEntity;

	OgreBites::ParamsPanel* mDetailsPanel;
	bool mQuit;

	Ogre::Vector3 mTranslateVector;
	Ogre::Real mMoveSpeed;
	Ogre::Degree mRotateSpeed;
	float mMoveScale;
	Ogre::Degree mRotScale;

	bool mLMouseDown, mRMouseDown;
	bool mSettingsMode;
};

#endif
