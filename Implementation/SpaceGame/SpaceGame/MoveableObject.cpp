#include "MoveableObject.h"

MoveableObject::MoveableObject(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass)
	: GameEntity(entName, meshName, isDestroyable)
{
	MASS = objMass;
	motionState = new MoveableObjMotionState(sceneNode);
}


MoveableObject::~MoveableObject()
{
}

MoveableObjMotionState* MoveableObject::getMotionState()
{
	return motionState;
}