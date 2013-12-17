/*
 * GameState.cpp partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 10/11/2013
 */
#include "GameState.h"

using namespace Ogre;

GameState::GameState() : player("Player", "cockpit2.mesh", true, 200, 400, 2, Ogre::Radian(0.5f), Ogre::Radian(0.5f), Ogre::Radian(0.5f), 1.5f, 0.8f)
{
	mLMouseDown = false;
	mRMouseDown = false;
	mQuit = false;
	mDetailsPanel = 0;
	mAlreadyInit = false;
	mSceneMgr = Engine::getSingletonPtr()->mSceneManager;
}

void GameState::enter()
{
	Engine::getSingletonPtr()->mLog->logMessage("Entering GameState...");
	Engine::getSingletonPtr()->mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	setupGUI();
	createScene(Engine::getSingletonPtr()->playerNode);
#ifdef OGRE_EXTERNAL_OVERLAY
	//mSceneMgr->addRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif
}

bool GameState::pause()
{
	Engine::getSingletonPtr()->mLog->logMessage("Pausing");
	return true;
}

void GameState::resume()
{
	Engine::getSingletonPtr()->mLog->logMessage("Resuming");
	setupGUI();
	//Engine::getSingletonPtr()->mViewport->setCamera(mCamera);
	mQuit = false;
}

void GameState::exit()
{
	Engine::getSingletonPtr()->mLog->logMessage("Leaving GameState");
#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->removeRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif
	//mSceneMgr->destroyCamera(mCamera);
	//if(mSceneMgr)
		//Engine::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);
}

void GameState::createScene(Ogre::SceneNode* playerNode)
{
	mSceneMgr->getRootSceneNode()->removeChild("Player_Node");
	playerNode->addChild(player.getSceneNode());

	if(!mAlreadyInit)
	{
		mSceneMgr->createLight("Light")->setPosition(75,75,75);
	
		/*mOgreHeadEntity = mSceneMgr->createEntity("OgreHeadEntity", "ogrehead.mesh");
		mOgreHeadNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode");
		mOgreHeadNode->attachObject(mOgreHeadEntity);
		mOgreHeadNode->setPosition(Vector3(0, 0, -25));

		mOgreHeadMat = mOgreHeadEntity->getSubEntity(1)->getMaterial();
		mOgreHeadMatHigh = mOgreHeadMat->clone("OgreHeadMatHigh");
		mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
		mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);*/
	}
	mAlreadyInit = true;
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		pushState(findByName("PauseState"));
		return true;
	}

	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_I))
	{
		if(mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			Engine::getSingletonPtr()->mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPLEFT, 0);
			mDetailsPanel->show();
		}
		else
		{
			Engine::getSingletonPtr()->mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
	}

	return true;
}

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	Engine::getSingletonPtr()->keyPressed(keyEventRef);
	return true;
}

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseMove(evt)) return true;
	return true;
}

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseDown(evt, id)) return true;

	if(id == OIS::MB_Left)
	{
		//onLeftPressed(evt);
		mLMouseDown = true;
	}
	else if(id == OIS::MB_Right)
	{
		mRMouseDown = true;
	}

	return true;
}

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseUp(evt, id)) return true;

	if(id == OIS::MB_Left)
	{
		mLMouseDown = false;
	}
	else if(id == OIS::MB_Right)
	{
		mRMouseDown = false;
	}
	return true;
}

void GameState::getInput()
{
	
}

void GameState::update(double timeSinceLastFrame)
{
	mFrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	Engine::getSingletonPtr()->mTrayMgr->frameRenderingQueued(mFrameEvent);

	if(mQuit == true)
	{
		popState();
		return;
	}

	getInput();
	player.update(timeSinceLastFrame);
}

void GameState::setupGUI()
{
	Engine::getSingletonPtr()->mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	Engine::getSingletonPtr()->mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	Engine::getSingletonPtr()->mTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Game mode", 250);
	Engine::getSingletonPtr()->mTrayMgr->showCursor();

	Ogre::StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("Mode");

	mDetailsPanel = Engine::getSingletonPtr()->mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel", 200, items);
	mDetailsPanel->show();

	Ogre::String infoText = "[TAB] - Switch input mode\n\n[W] - Forward / Mode up\n[S] - Backwards/ Mode down\n[A] - Left\n";
	infoText.append("[D] - Right\n\nPress [SHIFT] to move faster\n\n[O] - Toggle FPS / logo\n");
	infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
	Engine::getSingletonPtr()->mTrayMgr->createTextBox(OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 220);

	Ogre::StringVector chatModes;
	chatModes.push_back("Solid mode");
	chatModes.push_back("Wireframe mode");
	chatModes.push_back("Point mode");
	Engine::getSingletonPtr()->mTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "ChatMode", 200, 3, chatModes);
}