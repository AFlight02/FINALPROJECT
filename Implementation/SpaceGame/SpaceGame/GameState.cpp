/*
 * GameState.cpp partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 10/11/2013
 */
#include "GameState.h"

using namespace Ogre;

GameState::GameState()
{
	mMoveSpeed = 0.1f;
	mRotateSpeed = 0.3f;
	mLMouseDown = false;
	mRMouseDown = false;
	mQuit = false;
	mSettingsMode = false;
	mDetailsPanel = 0;
}

void GameState::enter()
{
	Engine::getSingletonPtr()->mLog->logMessage("Entering GameState...");

	mSceneMgr = Engine::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	mCamera = mSceneMgr->createCamera("GameCamera");
	mCamera->setPosition(Vector3(5, 60, 60));
	mCamera->lookAt(Vector3(5, 20, 0));
	mCamera->setNearClipDistance(5);
	mCamera->setAspectRatio(Real(Engine::getSingletonPtr()->mViewport->getActualWidth()) /
		Real(Engine::getSingletonPtr()->mViewport->getActualHeight()));

	Engine::getSingletonPtr()->mViewport->setCamera(mCamera);

	setupGUI();
	createScene();
#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->addRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
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
	Engine::getSingletonPtr()->mViewport->setCamera(mCamera);
	mQuit = false;
}

void GameState::exit()
{
	Engine::getSingletonPtr()->mLog->logMessage("Leaving GameState...");
#ifdef OGRE_EXTERNAL_OVERLAY
	mSceneMgr->removeRenderQueueListener(Engine::getSingletonPtr()->mOverlaySystem);
#endif
	mSceneMgr->destroyCamera(mCamera);
	if(mSceneMgr)
		Engine::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);
}

void GameState::createScene()
{
	mSceneMgr->createLight("Light")->setPosition(75,75,75);
	
	cubeEntity = mSceneMgr->createEntity("Cube1", Ogre::SceneManager::PrefabType::PT_CUBE);
	cubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Cube1Node");
	cubeNode->attachObject(cubeEntity);
	cubeNode->setPosition(Vector3(0,0,-25));
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(mSettingsMode == true)
	{
		if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_S))
		{
			OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)Engine::getSingletonPtr()->mTrayMgr->getWidget("ChatModeSelMenu");
			if(pMenu->getSelectionIndex() + 1 < (int)pMenu->getNumItems())
				pMenu->selectItem(pMenu->getSelectionIndex() + 1);
		}

		if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_W))
		{
			OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)Engine::getSingletonPtr()->mTrayMgr->getWidget("ChatModeSelMenu");
			if(pMenu->getSelectionIndex() - 1 >= 0)
				pMenu->selectItem(pMenu->getSelectionIndex() - 1);
		}
	}

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

	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_TAB))
	{
		mSettingsMode = !mSettingsMode;
		return true;
	}

	if(mSettingsMode && Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_RETURN) ||
		Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_NUMPADENTER))
	{
	}

	if(!mSettingsMode || (mSettingsMode && !Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_O)))
		Engine::getSingletonPtr()->keyPressed(keyEventRef);

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

	if(mRMouseDown)
	{
		mCamera->yaw(Degree(evt.state.X.rel * -0.1f));
		mCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
	}

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

void GameState::moveCamera()
{
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		mCamera->moveRelative(mTranslateVector);
	mCamera->moveRelative(mTranslateVector / 10);
}

void GameState::getInput()
{
	if(mSettingsMode == false)
	{
		if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_A))
			mTranslateVector.x = -mMoveScale;

		if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_D))
			mTranslateVector.x = mMoveScale;

		if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_W))
			mTranslateVector.z = -mMoveScale;

		if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_S))
			mTranslateVector.z = mMoveScale;
	}
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

	if(!Engine::getSingletonPtr()->mTrayMgr->isDialogVisible())
	{
		if(mDetailsPanel->isVisible())
		{
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
			mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
			mDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
			mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
			mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
			mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
			if(mSettingsMode)
				mDetailsPanel->setParamValue(7, "Buffered Input");
			else
				mDetailsPanel->setParamValue(7, "Un-Buffered Input");
		}
	}

	mMoveScale = mMoveSpeed   * timeSinceLastFrame;
	mRotScale  = mRotateSpeed * timeSinceLastFrame;

	mTranslateVector = Vector3::ZERO;

	getInput();
	moveCamera();
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

void GameState::itemSelected(OgreBites::SelectMenu* menu)
{
	switch(menu->getSelectionIndex())
	{
	case 0:
		mCamera->setPolygonMode(Ogre::PM_SOLID);break;
	case 1:
		mCamera->setPolygonMode(Ogre::PM_WIREFRAME);break;
	case 2:
		mCamera->setPolygonMode(Ogre::PM_POINTS);break;
	}
}