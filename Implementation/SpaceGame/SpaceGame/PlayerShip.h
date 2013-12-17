/*
 * PlayerShip.h class 
 * Author : Alex Flight
 * Version : 1.0
 * Date : 17/11/2013
 */
#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#pragma once

#include "Spacecraft.h"

class Spacecraft;

class PlayerShip : public Spacecraft,  public OIS::KeyListener, public OIS::MouseListener
{
public:
	PlayerShip(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, Ogre::Radian roll, Ogre::Radian pitch, Ogre::Radian yaw,
		float decceleration, float translate);
	~PlayerShip();

	void setupPhysics();

	void targetObject();
	void toggleAdvFlightMode();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void update(double timeSinceLastFrame);

protected:
	//HUD hud
	Ogre::Entity* target;
	Ogre::Radian currPitch;
	Ogre::Radian currYaw;
	Ogre::Radian currRoll;
	float currTranslateX;
	float currTranslateY;
	float currAcceleration;

	bool advFlightMode;
	float SHIELD_RECHARGE_RATE;

	btCollisionShape* shipShape;
	btDefaultMotionState* shipMotionState;
	btRigidBody* shipRigidBody;
	btDynamicsWorld* bulWorld;
};

#endif