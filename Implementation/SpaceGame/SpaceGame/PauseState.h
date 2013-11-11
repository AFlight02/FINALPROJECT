#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#pragma once

#include "EngineState.h"

class PauseState : public EngineState
{
public:
	PauseState();

	DECLARE_ENGINESTATE_CLASS(PauseState)

	void enter();
    void createScene();
    void exit();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

    void buttonHit(OgreBites::Button* button);
    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

    void update(double timeSinceLastFrame);

private:
    bool mQuit;
    bool mQuestionActive;
};

#endif