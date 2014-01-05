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
	btRigidBody* getRigidBody();

	void targetObject();
	void toggleAdvFlightMode();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void update(double timeSinceLastFrame);

	float currPitch;
	float currYaw;
	float currRoll;
	float currTranslateX;
	float currTranslateY;
	float currTranslateZ;

	btVector3 translation;
	btVector3 rotation;

protected:
	//HUD hud
	Ogre::Entity* target;

	bool advFlightMode;
	float SHIELD_RECHARGE_RATE;

	btCollisionShape* shipShape;
	MoveableObjMotionState* shipMotionState;
	btRigidBody* shipRigidBody;
};

#endif