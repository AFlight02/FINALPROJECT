/*
 * Engine.cpp partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 09/11/2013
 */
#include "Engine.h"

using namespace Ogre;

template<> Engine* Ogre::Singleton<Engine>::msSingleton = 0;

// Default Constructor inits to 0
Engine::Engine(void)
{
	mRoot = 0;
	mRenderWnd = 0;
	mViewport = 0;
	mLog = 0;
	mTimer = 0;
	mInputMgr = 0;
	mKeyboard = 0;
	mMouse = 0;
#ifdef OGRE_EXTERNAL_OVERLAY
	mOverlaySystem = 0;
#endif
	mTrayMgr = 0;
}

// Default Destructor deletes SDK Tray Manager, Overlay, Input and the Root
Engine::~Engine(void)
{
	Engine::getSingletonPtr()->mLog->logMessage("Exiting OGRE");
	if(mTrayMgr) delete mTrayMgr;
#ifdef OGRE_EXTERNAL_OVERLAY
	if(mOverlaySystem) delete mOverlaySystem;
#endif
	if(mInputMgr) OIS::InputManager::destroyInputSystem(mInputMgr);
	if(mRoot) delete mRoot;
}

// Initialises Ogre with default values. Pass a window title, and pointers to Key and Mouse Listeners
bool Engine::initOgreRenderer(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
	// Handle Logging
	Ogre::LogManager* logMgr = new Ogre::LogManager();
	mLog = Ogre::LogManager::getSingleton().createLog("SpaceGameLogFile.log", true, true, false);
	mLog->setDebugOutputEnabled(true);

	// Load debug plugins if debug enabled
#ifdef _DEBUG
	mRoot = new Ogre::Root("plugins_d_space_game.cfg");
#else
	mRoot = new Ogre::Root("plugins_space_game.cfg");
#endif
	// Load Overlay if supported
#ifdef OGRE_EXTERNAL_OVERLAY
	mOverlaySystem = new Ogre::OverlaySystem();
#endif

	// Init window and viewport
	if(!mRoot->showConfigDialog()) return false;
	mRenderWnd = mRoot->initialise(true, wndTitle);
	mSceneManager = mRoot->createSceneManager("OctreeSceneManager");

	Ogre::String sectionName, typeName, valueName;
	Ogre::ConfigFile config;

	// Load correct resources config
#ifdef _DEBUG
	config.load("resources_d_space_game.cfg");
#else
	config.load("resources_space_game.cfg");
#endif

	// Load Ogre Resources from config file and setup Resource and Texture manager
	Ogre::ConfigFile::SectionIterator secIt = config.getSectionIterator();
	while(secIt.hasMoreElements())
	{
		sectionName = secIt.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			valueName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(valueName, typeName, sectionName);
		}
	}
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	oculus.setupOculus();
	oculus.setupOgre(mSceneManager, mRenderWnd);
	oculus.getCameraNode()->setPosition(0.0f, 1.7f, 10.0f);

	//mViewport = mRenderWnd->addViewport(0);
	//mViewport->setBackgroundColour(ColourValue(0,0,0,1.0f)); // Set Viewport BG to BLACK 0,0,0,1
	//mViewport->setCamera(0);

	size_t hWnd = 0;
	std::ostringstream wndHndStr;
	OIS::ParamList paramList;
	// Get Window Title from WINDOWS Subsystem
	mRenderWnd->getCustomAttribute("WINDOW", &hWnd);
	wndHndStr << hWnd;
	paramList.insert(std::make_pair(std::string("WINDOW"), wndHndStr.str())); // Convert WINDOW title to string
	
	mInputMgr = OIS::InputManager::createInputSystem(paramList); // Create the input manager for the window defined in paramList
	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true)); // Create the keyboard
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true)); // Create the mouse

	// Set Mouse width/height
	mMouse->getMouseState().height = mRenderWnd->getHeight();
	mMouse->getMouseState().width = mRenderWnd->getWidth();

	// Setup mouse/keyboard event callbacks to this if no listeners exist
	if(pKeyListener == 0)
		mKeyboard->setEventCallback(this);
	else
		mKeyboard->setEventCallback(pKeyListener);
	if(pMouseListener == 0)
		mMouse->setEventCallback(this);
	else
		mMouse->setEventCallback(pMouseListener);

	// Get Overlay input contexts
#ifdef OGRE_EXTERNAL_OVERLAY
	OgreBites::InputContext input;
	input.mAccelerometer = NULL;
	input.mKeyboard = mKeyboard;
	input.mMouse = mMouse;
	input.mMultiTouch = NULL;
	mTrayMgr = new OgreBites::SdkTrayManager("SpaceGameTrayMgr", mRenderWnd, input, 0);
#else
	mTrayMgr = new OgreBites::SdkTrayManager("SpaceGameTrayMgr", mRenderWnd, mMouse, 0);
#endif
	// Set up the timer and activate the render window
	mTimer = new Ogre::Timer();
	mTimer->reset();

	mRenderWnd->setActive(true);
	return true;
}

// Some default key press handlers for debug purposes
bool Engine::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(mKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		mRenderWnd->writeContentsToTimestampedFile("SpaceGame_Screenshot_", ".jpg");
		return true;
	}

	if(mKeyboard->isKeyDown(OIS::KC_O))
	{
		if(mTrayMgr->isLogoVisible())
		{
			mTrayMgr->hideFrameStats();
			mTrayMgr->hideLogo();
		}
		else
		{
			mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
			mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
		}
	}

	return true;
}

bool Engine::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

bool Engine::mouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}

bool Engine::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}


bool Engine::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

// Update with time since last frame
void Engine::updateOgre(double timeSinceLastFrame)
{
}
