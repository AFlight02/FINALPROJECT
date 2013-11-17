#include "MoveableObject.h"

MoveableObject::MoveableObject(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass)
	: GameEntity(entName, meshName, isDestroyable)
{
	MASS = objMass;
}


MoveableObject::~MoveableObject()
{
}

void MoveableObject::accelerate()
{
	getSceneNode()->translate(velocityVector);
}