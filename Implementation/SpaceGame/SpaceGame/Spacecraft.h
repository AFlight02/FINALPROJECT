/*
 * Spacecraft.h class 
 * Author : Alex Flight
 * Version : 1.0
 * Date : 17/11/2013
 */
#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#pragma once

#include "MoveableObject.h"

class MoveableObject; //Forward declaration

class Spacecraft : public MoveableObject
{
public:
	Spacecraft(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, float roll, float pitch, float yaw,
		float decceleration, float translate);
	~Spacecraft();

	float getShields();
	float getHull();
	
	void pitch(float amount);
	void roll(float amount);
	void yaw(float amount);
	void thrust(float amount);
	void translateX(float amount);
	void translateY(float amount);
	
	void applyDamage(float amount);
	void fireWeaponPrimary();
	void fireWeaponSecondary();

	void update(double timeSinceLastFrame);
	
	void setupPhysics();
	btRigidBody* getRigidBody();
	btVector3 getTranslationVector();
	btVector3 getRotationVector();

protected:
	btRigidBody* shipRigidBody;
	btVector3 translation;
	btVector3 rotation;

	float ROLL_RATE;
    float YAW_RATE;
	float PITCH_RATE;
	float MAX_VELOCITY;
	float MAX_ACCELERATION;
	float DECCELERATION_RATE;
	float TRANSLATE_RATE;
	float MAX_SHIELDS;
	float MAX_HULL;

	float shields;
	float hull;

	float currPitch;
	float currYaw;
	float currRoll;
	float currTranslateX;
	float currTranslateY;
	float currTranslateZ;

	//Weapon primaryWeapon; TODO
	//Weapon secondaryWeapon; TODO
};

#endif