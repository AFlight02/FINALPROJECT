/*
 * MoveableObject.h class 
 * Author : Alex Flight
 * Version : 1.0
 * Date : 17/11/2013
 */
#ifndef MOVEABLE_OBJECT_H
#define MOVEABLE_OBJECT_H

#pragma once

#include "GameEntity.h"
#include "Engine.h"

class GameEntity;

class MoveableObject : public GameEntity
{
public:
	MoveableObject(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass);
	~MoveableObject();

	BtOgre::RigidBodyState* getMotionState();
protected:
	float MASS;
	BtOgre::RigidBodyState* motionState;
	btCollisionShape* shape;
};

#endif