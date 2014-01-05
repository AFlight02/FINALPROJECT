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
	Engine::getSingletonPtr()->mLog->logMessage("\nPausing");
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
	
		mOgreHeadEntity = mSceneMgr->createEntity("OgreHeadEntity", "ogrehead.mesh");
		mOgreHeadNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode");
		mOgreHeadNode->attachObject(mOgreHeadEntity);
		mOgreHeadNode->setPosition(Vector3(0, 0, -205));

		mOgreHeadMat = mOgreHeadEntity->getSubEntity(1)->getMaterial();
		mOgreHeadMatHigh = mOgreHeadMat->clone("OgreHeadMatHigh");
		mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
		mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
	}

	// Set up Bullet Physics
	bulBroadphase = new btDbvtBroadphase();
	bulCollisionConfiguration = new btDefaultCollisionConfiguration();
	bulDispatcher = new btCollisionDispatcher(bulCollisionConfiguration);
	bulSolver = new btSequentialImpulseConstraintSolver;
	bulDynamicsWorld = new btDiscreteDynamicsWorld(bulDispatcher, bulBroadphase, bulSolver, bulCollisionConfiguration);
	bulDynamicsWorld->setGravity(btVector3(0,0,0));

	torque = btVector3(0,0,0);
	//shipRigidBody = player.getRigidBody();
	player.setupPhysics();
	bulDynamicsWorld->addRigidBody(player.getRigidBody());
	mAlreadyInit = true;
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	switch(keyEventRef.key) {
		case(OIS::KC_ESCAPE):
			Engine::getSingletonPtr()->mLog->logMessage("ESC Pressed");
			pushState(findByName("PauseState"));
			break;
		case(OIS::KC_UP):
			Engine::getSingletonPtr()->mLog->logMessage("UP Pressed");
			player.currPitch -= 10;
			//shipRigidBody->applyTorqueImpulse(shipRigidBody->getInvInertiaTensorWorld().inverse()*(shipRigidBody->getWorldTransform().getBasis()*btVector3(0, 100000000, 0)));
			//player.getRigidBody()->applyForce(btVector3(0,10,0), player.getRigidBody()->getCenterOfMassPosition());
			break;
		case(OIS::KC_DOWN):
			player.currPitch += 10;
			//shipRigidBody->applyTorque(btVector3(0, -10, 0));
			//player.getRigidBody()->applyTorque(btVector3(0,-10,0));
			break;
		case(OIS::KC_LEFT):
			player.currYaw += 10;
			//shipRigidBody->applyTorque(btVector3(10, 0, 0));
			//player.getRigidBody()->applyTorque(btVector3(10,0,0));
			break;
		case(OIS::KC_RIGHT):
			player.currYaw -= 10;
			//shipRigidBody->applyTorque(btVector3(-10, 0, 0));
			//player.getRigidBody()->applyTorque(btVector3(-10,0,0));
			break;
		case(OIS::KC_Q):
			player.currRoll += 10;
			//shipRigidBody->applyTorque(btVector3(0, 0, 10));
			//player.getRigidBody()->applyTorque(btVector3(0,0,10));
			break;
		case(OIS::KC_E):
			player.currRoll -= 10;
			//shipRigidBody->applyTorque(btVector3(0, 0, -10));
			//player.getRigidBody()->applyTorque(btVector3(0,0,-10));
			break;
		case(OIS::KC_W):
			Engine::getSingletonPtr()->mLog->logMessage("W Pressed");
			player.currTranslateY += 100;
			//shipRigidBody->applyCentralImpulse(btVector3(0,10,0));
			//player.getRigidBody()->applyImpulse(btVector3(0,10,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_S):
			player.currTranslateY -= 100;
			//shipRigidBody->applyCentralImpulse(btVector3(0,-10,0));
			//player.getRigidBody()->applyImpulse(btVector3(0,-10,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_A):
			player.currTranslateX -=100;
			//shipRigidBody->applyCentralImpulse(btVector3(10,0,0));
			//player.getRigidBody()->applyImpulse(btVector3(10,0,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_D):
			player.currTranslateX += 100;
			//shipRigidBody->applyCentralImpulse(btVector3(-10,0,0));
			//player.getRigidBody()->applyImpulse(btVector3(10,0,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_LSHIFT):
			player.currTranslateZ -= 250;
			//shipRigidBody->applyCentralImpulse(btVector3(0,0,20));
			//player.getRigidBody()->applyCentralImpulse(player.getRigidBody()->getWorldTransform().getBasis().getColumn(2) * 20);
			break;
		case(OIS::KC_LCONTROL):
			player.currTranslateZ += 250;
			//shipRigidBody->applyCentralImpulse(btVector3(0,0,-20));
			//player.getRigidBody()->applyCentralImpulse(player.getRigidBody()->getWorldTransform().getBasis().getColumn(2) * -20);
			break;
		default:
			break;
	}
	return true;
}

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	Engine::getSingletonPtr()->keyPressed(keyEventRef);
		switch(keyEventRef.key) {
		case(OIS::KC_UP):
			Engine::getSingletonPtr()->mLog->logMessage("UP Pressed");
			player.currPitch = 0;
			//shipRigidBody->applyTorqueImpulse(shipRigidBody->getInvInertiaTensorWorld().inverse()*(shipRigidBody->getWorldTransform().getBasis()*btVector3(0, 100000000, 0)));
			//player.getRigidBody()->applyForce(btVector3(0,10,0), player.getRigidBody()->getCenterOfMassPosition());
			break;
		case(OIS::KC_DOWN):
			player.currPitch = 0;
			//shipRigidBody->applyTorque(btVector3(0, -10, 0));
			//player.getRigidBody()->applyTorque(btVector3(0,-10,0));
			break;
		case(OIS::KC_LEFT):
			player.currYaw = 0;
			//shipRigidBody->applyTorque(btVector3(10, 0, 0));
			//player.getRigidBody()->applyTorque(btVector3(10,0,0));
			break;
		case(OIS::KC_RIGHT):
			player.currYaw = 0;
			//shipRigidBody->applyTorque(btVector3(-10, 0, 0));
			//player.getRigidBody()->applyTorque(btVector3(-10,0,0));
			break;
		case(OIS::KC_Q):
			player.currRoll = 0;
			//shipRigidBody->applyTorque(btVector3(0, 0, 10));
			//player.getRigidBody()->applyTorque(btVector3(0,0,10));
			break;
		case(OIS::KC_E):
			player.currRoll = 0;
			//shipRigidBody->applyTorque(btVector3(0, 0, -10));
			//player.getRigidBody()->applyTorque(btVector3(0,0,-10));
			break;
		case(OIS::KC_W):
			Engine::getSingletonPtr()->mLog->logMessage("W Pressed");
			player.currTranslateY = 0;
			//shipRigidBody->applyCentralImpulse(btVector3(0,10,0));
			//player.getRigidBody()->applyImpulse(btVector3(0,10,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_S):
			player.currTranslateY = 0;
			//shipRigidBody->applyCentralImpulse(btVector3(0,-10,0));
			//player.getRigidBody()->applyImpulse(btVector3(0,-10,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_A):
			player.currTranslateX = 0;
			//shipRigidBody->applyCentralImpulse(btVector3(10,0,0));
			//player.getRigidBody()->applyImpulse(btVector3(10,0,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_D):
			player.currTranslateX = 0;
			//shipRigidBody->applyCentralImpulse(btVector3(-10,0,0));
			//player.getRigidBody()->applyImpulse(btVector3(10,0,0), player.getRigidBody()->getCenterOfMassTransform().getOrigin());
			break;
		case(OIS::KC_LSHIFT):
			player.currTranslateZ = 0;
			//shipRigidBody->applyCentralImpulse(btVector3(0,0,20));
			//player.getRigidBody()->applyCentralImpulse(player.getRigidBody()->getWorldTransform().getBasis().getColumn(2) * 20);
			break;
		case(OIS::KC_LCONTROL):
			player.currTranslateZ = 0;
			//shipRigidBody->applyCentralImpulse(btVector3(0,0,-20));
			//player.getRigidBody()->applyCentralImpulse(player.getRigidBody()->getWorldTransform().getBasis().getColumn(2) * -20);
			break;
		default:
			break;
	}
	return true;
}

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
	if(Engine::getSingletonPtr()->mTrayMgr->injectMouseMove(evt)) return true;
	//shipRigidBody->applyTorque(btVector3(-evt.state.X.rel, evt.state.Y.rel, 0));
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
	bulDynamicsWorld->stepSimulation(timeSinceLastFrame,5);
	
	btTransform trans;
	player.getMotionState()->getWorldTransform(trans);
	btQuaternion orientation = trans.getRotation();
	//player.getSceneNode()->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	//player.getSceneNode()->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
	StringConverter sc;
	//Engine::getSingletonPtr()->mLog->logMessage("Trans: "+sc.toString(trans.getOrigin().x()));
	//Engine::getSingletonPtr()->mLog->logMessage("Orient: "+sc.toString(orientation.x()));

	player.getRigidBody()->activate(true);
	btMatrix3x3& movement = player.getRigidBody()->getWorldTransform().getBasis();
	btVector3 correctedRot = movement*player.rotation;
	btVector3 correctedTrans = movement*player.translation;
	player.getRigidBody()->applyTorqueImpulse(correctedRot/1000);
	player.getRigidBody()->applyCentralImpulse(correctedTrans/100);
	//player.getRigidBody()->applyTorqueImpulse(btVector3(0, 10, 0));
	getInput();
	player.update(timeSinceLastFrame);
	//player.getRigidBody()->applyCentralForce(player.rotation);
	//player.getRigidBody()->applyTorqueImpulse(player.rotation);
	//player.getRigidBody()->applyCentralImpulse(player.translation);
	//torque = btVector3(0,0,0);
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