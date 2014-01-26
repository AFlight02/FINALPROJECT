#include "MoveableObject.h"

MoveableObject::MoveableObject(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass)
	: GameEntity(entName, meshName, isDestroyable)
{
	MASS = objMass;
	motionState = new BtOgre::RigidBodyState(sceneNode);
	BtOgre::StaticMeshToShapeConverter converter(entity);
	shape = converter.createSphere();
}


MoveableObject::~MoveableObject()
{
}

BtOgre::RigidBodyState* MoveableObject::getMotionState()
{
	return motionState;
}