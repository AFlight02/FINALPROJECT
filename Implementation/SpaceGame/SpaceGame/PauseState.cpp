/*
 * PauseState.cpp - Manages the game's paused state and displays a pause menu that
 * allows the player to resume, exit to menu or quit the application entirely.
 * Author: Alex Flight
 * Date: 10/11/2013
 */
#include "PauseState.h"

using namespace Ogre;

// Default constructor sets default values to false and gets a new FrameEvent object
PauseState::PauseState()
{
	mQuit = false;
	mQuestionActive = false;
	mFrameEvent = Ogre::FrameEvent();
}

// Called on entry to the PauseState, writes to log and creates appropriate UI/Menu elements, 
// then creates the pause menu scene.
void PauseState::enter()
{
	Engine::getSingletonPtr()->mLog->logMessage("Entering PauseState");

	//mSceneMgr = Engine::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "PauseSceneMgr");
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	/*
	mCamera = mSceneMgr->createCamera("PauseCam");
	mCamera->setPosition(Vector3(0, 25, -50));
	mCamera->lookAt(Vector3(0, 0, 0));
	mCamera->setNearClipDistance(1);

	mCamera->setAspectRatio(Real(Engine::getSingletonPtr()->mViewport->getActualWidth()) /
		Real(Engine::getSingletonPtr()->mViewport->getActualHeight()));

	Engine::getSingletonPtr()->mViewport->setCamera(mCamera);
*/
	Engine::getSingletonPtr()->mTrayMgr->destroyAllWidgets();
	Engine::getSingletonPtr()->mTrayMgr->showCursor();
	Engine::getSingletonPtr()->mTrayMgr->createButton(OgreBites::TL_CENTER, "BackToGameBtn", "Return to GameState", 250);
	Engine::getSingletonPtr()->mTrayMgr->createButton(OgreBites::TL_CENTER, "BackToMenuBtn", "Return to Menu", 250);
	Engine::getSingletonPtr()->mTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit Game", 250);
	Engine::getSingletonPtr()->mTrayMgr->createLabel(OgreBites::TL_TOP, "PauseLbl", "Pause mode", 250);

	mQuit = false;

	createScene(Engine::getSingletonPtr()->mSceneManager, Engine::getSingletonPtr()->mCameraNode);
// Case if external overlay is in use in Ogre v1.9 or greater
#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->addRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif
}

/* 
 * CreateScene takes pointers to the SceneManager and CameraNodes
 * @param sm Pointer to the game's SceneManager
 * @param cameraNode Pointer to the Oculus SceneNode that manages the stereo cameras.
 */
void PauseState::createScene(Ogre::SceneManager* sm, Ogre::SceneNode* cameraNode)
{
	mSceneMgr = sm;
	mCameraNode = cameraNode;
}

// Called on exit from PauseState class, clears UI elements exclusive to the Pause Menu.
void PauseState::exit()
{
	Engine::getSingletonPtr()->mLog->logMessage("Leaving PauseState");
#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->removeRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif

	/*mSceneMgr->destroyCamera(mCamera);
	if(mSceneMgr)
		Engine::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);*/

	Engine::getSingletonPtr()->mTrayMgr->clearAllTrays();
	Engine::getSingletonPtr()->mTrayMgr->destroyAllWidgets();
	Engine::getSingletonPtr()->mTrayMgr->setListener(0);
}

/* 
 * Key Pressed KeyEvent handler responds to key press events from the OIS Keyboard object.
 * @param &keyEventRef Address of the KeyEvent that was fired by the OIS Keyboard system
 * @return true in all cases
 */
bool PauseState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_ESCAPE) && !mQuestionActive)
	{
		mQuit = true;
		return true;
	}

	Engine::getSingletonPtr()->keyPressed(keyEventRef);
	return true;
}

/* 
 * Key Released event handler detects key release events from OIS Keyboard
 * @param &keyEventRef Address of the key handler that fired the Key Event
 * @return true in all cases
 */
bool PauseState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	Engine::getSingletonPtr()->keyReleased(keyEventRef);
	return true;
}

/* 
 * Mouse Movement handler detects mouse movement events and injects them into the trayManager/Overlay
 * @param &evt Address of the mouse event fired
 */
bool PauseState::mouseMoved(const OIS::MouseEvent &evt)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseMove(evt)) return true;
	return true;
}

/*
 * MousePress event handler that detects and responds to mouse click events.
 * @param &evt Address of hook fired from OIS Mouse object
 * @param id Identity of the mouse button pressed 
 */
bool PauseState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseDown(evt, id)) return true;
	return true;
}

/* 
 * MouseReleased handler detects and responds to mouse release events.
 * @param &evt Address of mouse release event.
 * @param id Identity of mouse button that was released
 */
bool PauseState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseUp(evt, id)) return true;
	return true;
}

void PauseState::update(double timeSinceLastFrame)
{
	mFrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	Engine::getSingletonPtr()->mTrayMgr->frameRenderingQueued(mFrameEvent);

	if(mQuit == true)
	{
		popState();
		return;
	}
}

void PauseState::buttonHit(OgreBites::Button *button)
{
	if(button->getName() == "ExitBtn")
	{
		Engine::getSingletonPtr()->mTrayMgr->showYesNoDialog("Are You Sure?", "Really leave?");
		mQuestionActive = true;
	}
	else if(button->getName() == "BackToGameBtn")
		mQuit = true;
	else if(button->getName() == "BackToMenuBtn")
		popAllAndPushNewEngineState(findByName("MenuState"));
}

void PauseState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
	if(yesHit == true)
		shutdown();
	else
		Engine::getSingletonPtr()->mTrayMgr->closeDialog();
	mQuestionActive = false;
}
