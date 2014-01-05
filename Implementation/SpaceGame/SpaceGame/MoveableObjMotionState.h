#pragma once

#include "Engine.h"

class MoveableObjMotionState : public btMotionState
{
public:
	MoveableObjMotionState(Ogre::SceneNode *node);
	virtual ~MoveableObjMotionState();

	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);
protected:
	Ogre::SceneNode *mVisibleObject;
	btTransform mPosition;
};

