#include "MenuState.h"

using namespace Ogre;

MenuState::MenuState(void)
{
	mQuit= false;
	mFrameEvent = Ogre::FrameEvent();
}

void MenuState::enter()
{
	Engine::getSingletonPtr()->mLog->logMessage("Entering MenuState");

	mSceneMgr = Engine::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	mCamera = mSceneMgr->createCamera("MenuCam");
	mCamera->setPosition(Vector3(0, 25, -50));
	mCamera->lookAt(Vector3(0, 0, 0));
	mCamera->setNearClipDistance(1);

	mCamera->setAspectRatio(Real(Engine::getSingletonPtr()->mViewport->getActualWidth()) /
		Real(Engine::getSingletonPtr()->mViewport->getActualHeight()));

	Engine::getSingletonPtr()->mViewport->setCamera(mCamera);

	Engine::getSingletonPtr()->mTrayMgr->destroyAllWidgets();
	Engine::getSingletonPtr()->mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	Engine::getSingletonPtr()->mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	Engine::getSingletonPtr()->mTrayMgr->showCursor();
	Engine::getSingletonPtr()->mTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn", "Enter GameState", 250);
	Engine::getSingletonPtr()->mTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit AdvancedOgreFramework", 250);
	Engine::getSingletonPtr()->mTrayMgr->createLabel(OgreBites::TL_TOP, "MenuLbl", "Menu mode", 250);

	createScene();

#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->addRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif
}

void MenuState::createScene()
{
}

void MenuState::exit()
{
	Engine::getSingletonPtr()->mLog->logMessage("Leaving MenuState");
#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->removeRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif

	mSceneMgr->destroyCamera(mCamera);
	if(mSceneMgr)
		Engine::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);

	Engine::getSingletonPtr()->mTrayMgr->clearAllTrays();
	Engine::getSingletonPtr()->mTrayMgr->destroyAllWidgets();
	Engine::getSingletonPtr()->mTrayMgr->setListener(0);
}

bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		mQuit = true;
		return true;
	}

	Engine::getSingletonPtr()->keyPressed(keyEventRef);
	return true;
}

bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	Engine::getSingletonPtr()->keyReleased(keyEventRef);
	return true;
}

bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseMove(evt)) return true;
	return true;
}

bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseDown(evt, id)) return true;
	return true;
}


bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseUp(evt, id)) return true;
	return true;
}

void MenuState::update(double timeSinceLastFrame)
{
	mFrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	Engine::getSingletonPtr()->mTrayMgr->frameRenderingQueued(mFrameEvent);

	if(mQuit == true)
	{
		shutdown();
		return;
	}
}

void MenuState::buttonHit(OgreBites::Button *button)
{
	if(button->getName() == "ExitBtn")
		mQuit = true;
	else if(button->getName() == "EnterBtn")
		changeState(findByName("GameState"));
}
