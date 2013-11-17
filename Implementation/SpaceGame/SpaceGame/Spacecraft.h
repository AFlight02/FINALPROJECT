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
	Spacecraft(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, Ogre::Radian roll, Ogre::Radian pitch, Ogre::Radian yaw,
		float decceleration, float translate);
	~Spacecraft();

	float getShields();
	float getHull();
	
	void pitch(Ogre::Radian amount);
	void roll(Ogre::Radian amount);
	void yaw(Ogre::Radian amount);
	void accelerate(float amount);
	void deccelerate(float amount);
	void translateX(float amount);
	void translateY(float amount);
	
	void applyDamage(float amount);
	void fireWeaponPrimary();
	void fireWeaponSecondary();
protected:
	Ogre::Radian ROLL_RATE;
    Ogre::Radian YAW_RATE;
	Ogre::Radian PITCH_RATE;
	float MAX_VELOCITY;
	float MAX_ACCELERATION;
	float DECCELERATION_RATE;
	float TRANSLATE_RATE;
	float MAX_SHIELDS;
	float MAX_HULL;

	float shields;
	float hull;

	//Weapon primaryWeapon; TODO
	//Weapon secondaryWeapon; TODO
};

#endif