#include "PauseState.h"

using namespace Ogre;

PauseState::PauseState()
{
	mQuit = false;
	mQuestionActive = false;
	mFrameEvent = Ogre::FrameEvent();
}

void PauseState::enter()
{
	Engine::getSingletonPtr()->mLog->logMessage("Entering PauseState");

	//mSceneMgr = Engine::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "PauseSceneMgr");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	/*mCamera = mSceneMgr->createCamera("PauseCam");
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

#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->addRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif
}

void PauseState::createScene(Ogre::SceneManager* sm, Ogre::SceneNode* cameraNode)
{
	mSceneMgr = sm;
	mCameraNode = cameraNode;
}

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

bool PauseState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	Engine::getSingletonPtr()->keyReleased(keyEventRef);
	return true;
}

bool PauseState::mouseMoved(const OIS::MouseEvent &evt)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseMove(evt)) return true;
	return true;
}

bool PauseState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseDown(evt, id)) return true;
	return true;
}

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
