#ifndef MENU_STATE_H
#define MENU_STATE_H

#pragma once

#include "EngineState.h"

class MenuState : public EngineState
{
public:
	MenuState();

	DECLARE_ENGINESTATE_CLASS(MenuState)

	void enter();
	void createScene(Ogre::SceneManager* sm, Ogre::SceneNode* cameraNode);
	void exit();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	void buttonHit(OgreBites::Button* button);
	void update(double timeSinceLastFrame);

private:
	bool mQuit;
};

#endif