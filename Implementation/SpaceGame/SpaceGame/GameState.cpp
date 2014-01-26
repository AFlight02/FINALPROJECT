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
	mLMouseDown = false;
	mRMouseDown = false;
	mQuit = false;
	mDetailsPanel = 0;
	mAlreadyInit = false;
	mSceneMgr = Engine::getSingletonPtr()->mSceneManager;
	physEngine = new Physics();
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
	if(!mAlreadyInit)
	{
		player = new PlayerShip("Player", "cockpit2.mesh", true, 2000, 400, 25, 5, 5, 5, 25, 15);
		testEnemy1 = new Enemy("Enemy1", "enemyShip1.mesh", true, 200, 400, 25, 5, 5, 5, 25, 15);
		mSceneMgr->getRootSceneNode()->removeChild("PlayerNode");
		player->getSceneNode()->addChild(playerNode);
		mSceneMgr->createLight("Light")->setPosition(10000,10000,10000);
	
		//mOgreHeadEntity = mSceneMgr->createEntity("OgreHeadEntity", "ogrehead.mesh");
		//mOgreHeadNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode");
		//mOgreHeadNode->attachObject(mOgreHeadEntity);
		//mOgreHeadNode->setPosition(Vector3(0, 0, -205));

		//mOgreHeadMat = mOgreHeadEntity->getSubEntity(1)->getMaterial();
		//mOgreHeadMatHigh = mOgreHeadMat->clone("OgreHeadMatHigh");
		//mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
		//mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
	}

	// Set up Bullet Physics
	// NOW DONE EXTERNALLY!
	//bulBroadphase = new btDbvtBroadphase();
	//bulCollisionConfiguration = new btDefaultCollisionConfiguration();
	//bulDispatcher = new btCollisionDispatcher(bulCollisionConfiguration);
	//bulSolver = new btSequentialImpulseConstraintSolver;
	//bulDynamicsWorld = new btDiscreteDynamicsWorld(bulDispatcher, bulBroadphase, bulSolver, bulCollisionConfiguration);
	//bulDynamicsWorld->setGravity(btVector3(0,0,0));

	//torque = btVector3(0,0,0);
	//shipRigidBody = player.getRigidBody();
	player->setupPhysics();
	//bulDynamicsWorld->addRigidBody(player.getRigidBody());
	physEngine->addRigidBody(player->getRigidBody());

	testEnemy1->getSceneNode()->setScale(10, 10, 10);
	testEnemy1->setupPhysics();
	testEnemy1->setPosition(Vector3(0,0, -500));
	//bulDynamicsWorld->addRigidBody(testEnemy1->getRigidBody());
	physEngine->addRigidBody(testEnemy1->getRigidBody());

	timer = new Timer();
	
	mSceneMgr->setDisplaySceneNodes(true);

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
			player->pitch(-1.0f);
			//player.currPitch -= 10;
			break;
		case(OIS::KC_DOWN):
			player->pitch(1.0f);
			//player.currPitch += 10;
			break;
		case(OIS::KC_LEFT):
			player->yaw(1.0f);
			//player.currYaw += 10;
			break;
		case(OIS::KC_RIGHT):
			player->yaw(-1.0f);
			//player.currYaw -= 10;
			break;
		case(OIS::KC_Q):
			player->roll(1.0f);
			//player.currRoll += 10;
			break;
		case(OIS::KC_E):
			player->roll(-1.0f);
			//player.currRoll -= 10;
			break;
		case(OIS::KC_W):
			player->translateY(1.0f);
			//player.currTranslateY += 100;
			break;
		case(OIS::KC_S):
			player->translateY(-1.0f);
			//player.currTranslateY -= 100;
			break;
		case(OIS::KC_A):
			player->translateX(-1.0f);
			//player.currTranslateX -=100;
			break;
		case(OIS::KC_D):
			player->translateX(1.0f);
			//player.currTranslateX += 100;
			break;
		case(OIS::KC_LSHIFT):
			player->thrust(-1.0f);
			//player.currTranslateZ -= 250;
			break;
		case(OIS::KC_LCONTROL):
			player->thrust(1.0f);
			//player.currTranslateZ += 250;
			break;
		// TESTING ENEMY PHYSICS
		case(OIS::KC_I):
			testEnemy1->thrust(1.0f);
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
			player->pitch(0);
			//player.currPitch = 0;
			break;
		case(OIS::KC_DOWN):
			player->pitch(0);
			//player.currPitch = 0;
			break;
		case(OIS::KC_LEFT):
			player->yaw(0);
			//player.currYaw = 0;
			break;
		case(OIS::KC_RIGHT):
			player->yaw(0);
			//player.currYaw = 0;
			break;
		case(OIS::KC_Q):
			player->roll(0);
			//player.currRoll = 0;
			break;
		case(OIS::KC_E):
			player->roll(0);
			//player.currRoll = 0;
			break;
		case(OIS::KC_W):
			player->translateY(0);
			//player.currTranslateY = 0;
			break;
		case(OIS::KC_S):
			player->translateY(0);
			//player.currTranslateY = 0;
			break;
		case(OIS::KC_A):
			player->translateX(0);
			//player.currTranslateX = 0;
			break;
		case(OIS::KC_D):
			player->translateX(0);
			//player.currTranslateX = 0;
			break;
		case(OIS::KC_LSHIFT):
			player->thrust(0);
			//player.currTranslateZ = 0;
			break;
		case(OIS::KC_LCONTROL):
			player->thrust(0);
			//player.currTranslateZ = 0;
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

	if(timer->getMilliseconds() > 2500)
	{
		// CHECKING STATES
		int currState = testEnemy1->getAIState();
		Ogre::String state;
		switch(currState)
		{
			case(0):
				state = "SEEK";
				Engine::getSingletonPtr()->mLog->logMessage("AI STATE - SEEK");
				break;
			case(1):
				state = "FLEE";
				Engine::getSingletonPtr()->mLog->logMessage("AI STATE - FLEE");
				break;
			default:
				state = "N/A";
				Engine::getSingletonPtr()->mLog->logMessage("AI STATE - NONE");
				break;
		}
		//CHECKING SCENENODES
		Ogre::StringConverter sc;
		Engine::getSingletonPtr()->mLog->logMessage("PLAYER POS " + sc.toString(player->getPosition()));
		Engine::getSingletonPtr()->mLog->logMessage("ENEMY1 POS " + sc.toString(testEnemy1->getPosition()));
		timer->reset();
	}

	getInput();

	if(mQuit == true)
	{
		popState();
		return;
	}
	physEngine->stepSimulation(timeSinceLastFrame,5);
	
	btTransform trans;
	player->getMotionState()->getWorldTransform(trans);
	player->getRigidBody()->activate(true);
	btMatrix3x3& movement = player->getRigidBody()->getWorldTransform().getBasis();
	btVector3 correctedTrans = movement * player->getTranslationVector();
	//player->getRigidBody()->applyTorqueImpulse(correctedRot/100);
	player->getRigidBody()->applyTorque(player->getRigidBody()->getInvInertiaTensorWorld().inverse() * player->getRigidBody()->getWorldTransform().getBasis() * player->getRotationVector() * .0001f);
	player->getRigidBody()->applyCentralImpulse(correctedTrans/100);
	player->update(timeSinceLastFrame);

	testEnemy1->updateAIState(player->getSceneNode()->getPosition(), player->getSceneNode()->getOrientation());
	btTransform enemyTrans;
	testEnemy1->getMotionState()->getWorldTransform(enemyTrans);
	//btQuaternion enemyOrientation = enemyTrans.getRotation();
	testEnemy1->getRigidBody()->activate(true);
	btMatrix3x3& enemyMovement = testEnemy1->getRigidBody()->getWorldTransform().getBasis();
	//btVector3 enemyCorrectedRot = enemyMovement * testEnemy1->getRotationVector();
	btVector3 enemyCorrectedTrans = enemyMovement * testEnemy1->getTranslationVector();
	//testEnemy1->getRigidBody()->applyTorqueImpulse(enemyCorrectedRot/100);
	testEnemy1->getRigidBody()->applyTorque(testEnemy1->getRigidBody()->getInvInertiaTensorWorld().inverse() * testEnemy1->getRigidBody()->getWorldTransform().getBasis() * testEnemy1->getRotationVector() * .0001f);
	testEnemy1->getRigidBody()->applyCentralImpulse(enemyCorrectedTrans/100);
	testEnemy1->update(timeSinceLastFrame);

	//if(!Engine::getSingletonPtr()->mTrayMgr->isDialogVisible())
	//{
	//	if(mDetailsPanel->isVisible())
	//	{
	//		int currState = testEnemy1->getAIState();
	//		Ogre::String state;
	//		switch(currState)
	//		{
	//		case(0):
	//			state = "SEEK";
	//			Engine::getSingletonPtr()->mLog->logMessage("AI STATE - SEEK");
	//			break;
	//		case(1):
	//			state = "FLEE";
	//			Engine::getSingletonPtr()->mLog->logMessage("AI STATE - FLEE");
	//			break;
	//		default:
	//			state = "N/A";
	//			Engine::getSingletonPtr()->mLog->logMessage("AI STATE - NONE");
	//			break;
	//		}
	//		mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(testEnemy1->getTranslationVector().z()));
	//		mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(testEnemy1->getRotationVector().y()));
	//		mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(testEnemy1->getRotationVector().z()));
	//		mDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(testEnemy1->getRotationVector().x()));
	//		mDetailsPanel->setParamValue(4, state);
	//	}
	//}
}

void GameState::setupGUI()
{
	Engine::getSingletonPtr()->mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);

	Ogre::StringVector items;
	items.push_back("EnThrust");
	items.push_back("EnPitch");
	items.push_back("EnRoll");
	items.push_back("EnYaw");
	items.push_back("EnAISTATE");

	mDetailsPanel = Engine::getSingletonPtr()->mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "AIDEBUG", 300, items);
	mDetailsPanel->show();
}