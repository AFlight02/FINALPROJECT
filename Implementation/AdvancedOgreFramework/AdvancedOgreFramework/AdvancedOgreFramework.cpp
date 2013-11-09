//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AdvancedOgreFramework.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
	m_pRoot				= 0;
	m_pRenderWnd		= 0;
	m_pViewport			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;

	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;
#ifdef OGRE_EXTERNAL_OVERLAY
	m_pOverlaySystem	= 0;
#endif
	m_pTrayMgr          = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
	if(m_pTrayMgr)      delete m_pTrayMgr;
#ifdef OGRE_EXTERNAL_OVERLAY
	if(m_pOverlaySystem) delete m_pOverlaySystem;
#endif
	if(m_pInputMgr)		OIS::InputManager::destroyInputSystem(m_pInputMgr);
	if(m_pRoot)			delete m_pRoot;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
	Ogre::LogManager* logMgr = new Ogre::LogManager();

	m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);

#ifdef _DEBUG
	m_pRoot = new Ogre::Root("plugins_d.cfg");
#else
	m_pRoot = new Ogre::Root("plugins.cfg");
#endif
#ifdef OGRE_EXTERNAL_OVERLAY
	m_pOverlaySystem = new Ogre::OverlaySystem();
#endif

	if(!m_pRoot->showConfigDialog())
		return false;
	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

	m_pViewport = m_pRenderWnd->addViewport(0);
	m_pViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

	m_pViewport->setCamera(0);

	size_t hWnd = 0;
	OIS::ParamList paramList;
	m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

	if(pKeyListener == 0)
		m_pKeyboard->setEventCallback(this);
	else
		m_pKeyboard->setEventCallback(pKeyListener);

	if(pMouseListener == 0)
		m_pMouse->setEventCallback(this);
	else
		m_pMouse->setEventCallback(pMouseListener);

	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
#ifdef _DEBUG
	cf.load("resources_d.cfg");
#else
	cf.load("resources.cfg");
#endif

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

#ifdef OGRE_EXTERNAL_OVERLAY
	OgreBites::InputContext input;
	input.mAccelerometer = NULL;
	input.mKeyboard = m_pKeyboard;
	input.mMouse = m_pMouse;
	input.mMultiTouch = NULL;

	m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", m_pRenderWnd, input, 0);
#else
	m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", m_pRenderWnd, m_pMouse, 0);
#endif

	m_pTimer = new Ogre::Timer();
	m_pTimer->reset();

	m_pRenderWnd->setActive(true);

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		m_pRenderWnd->writeContentsToTimestampedFile("AOF_Screenshot_", ".jpg");
		return true;
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		if(m_pTrayMgr->isLogoVisible())
		{
			m_pTrayMgr->hideFrameStats();
			m_pTrayMgr->hideLogo();
		}
		else
		{
			m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
			m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
		}
	}

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||