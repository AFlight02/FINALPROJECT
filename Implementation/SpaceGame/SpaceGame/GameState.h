/*
 * GameState.h partially adapted from 'AdvancedOgreFramework'
 * Author : Alex Flight
 * Version : 1.0
 * Date : 10/11/2013
 */
#ifndef GAME_STATE_H
#define GAME_STATE_H

#pragma once

#include "EngineState.h"
#include "PlayerShip.h"
#include "Celestial.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

class GameState : public EngineState
{
public:
	GameState();
	DECLARE_ENGINESTATE_CLASS(GameState)

	void enter();
	void createScene(Ogre::SceneNode* playerNode);
	void exit();
	bool pause();
	void resume();

	void getInput();
	void setupGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void update(double timeSinceLastFrame);

private:
	Ogre::SceneNode* mOgreHeadNode;
	Ogre::Entity* mOgreHeadEntity;
	Ogre::MaterialPtr mOgreHeadMat;
	Ogre::MaterialPtr mOgreHeadMatHigh;
	OgreBites::ParamsPanel* mDetailsPanel;

	PlayerShip player;

	// Bullet Resources
	btBroadphaseInterface* bulBroadphase;
	btDefaultCollisionConfiguration* bulCollisionConfiguration;
	btCollisionDispatcher* bulDispatcher;
	btSequentialImpulseConstraintSolver* bulSolver;
	btDiscreteDynamicsWorld* bulDynamicsWorld;

	btRigidBody* shipRigidBody;

	bool mQuit;
	bool mLMouseDown, mRMouseDown;
	bool mAlreadyInit;

	btVector3 torque;
};

#endif
