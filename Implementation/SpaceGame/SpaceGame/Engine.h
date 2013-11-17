/*
 * Engine.h partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 09/11/2013
 */
#ifndef ENGINE_H
#define ENGINE_H

#pragma once

// Include Oculus Integration class
#include "Oculus.h"

// Include Ogre headers
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

// Include OIS Headers
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

// Include OGRE SDK Tray header
#include <SdkTrays.h>

// Use new OGRE Overlay if OGRE Version is >= 1.9
#if OGRE_VERSION >= ((1 << 16) | (9 << 8) | 0)
    #define OGRE_EXTERNAL_OVERLAY
    #ifdef _MSC_VER
        #ifdef _DEBUG
            #pragma comment(lib, "OgreOverlay_d.lib")
        #else
            #pragma comment(lib, "OgreOverlay.lib")
        #endif
    #endif
#endif

// Extends Ogre Singleton, KeyListener and MouseListener
class Engine : public Ogre::Singleton<Engine>, OIS::KeyListener, OIS::MouseListener
{
public:
	Engine();
	~Engine();

	bool initOgreRenderer(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	void updateOgre(double timeSinceLastFrame);

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	// Define core Ogre resources
	Ogre::Root* mRoot;
	Ogre::SceneManager* mSceneManager;
	Ogre::RenderWindow* mRenderWnd;
	Ogre::Viewport* mViewport;
	Ogre::Log* mLog;
	Ogre::Timer* mTimer;
	Ogre::SceneNode* mCameraNode;
	Ogre::SceneNode* playerNode;

	// And OIS resources
	OIS::InputManager* mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;

	// Oculus integration handler class
	Oculus oculus;

	// Setup Overlay if External
#ifdef OGRE_EXTERNAL_OVERLAY
    Ogre::OverlaySystem* mOverlaySystem;
#endif
    OgreBites::SdkTrayManager* mTrayMgr;

	private:
    Engine(const Engine&);
    Engine& operator= (const Engine&);
};

#endif // !ENGINE_H