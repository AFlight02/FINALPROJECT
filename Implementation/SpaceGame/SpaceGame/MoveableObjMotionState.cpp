#include "MoveableObjMotionState.h"

MoveableObjMotionState::MoveableObjMotionState ( Ogre::SceneNode * node )
{
   mVisibleObject = node->getParentSceneNode();
   Ogre::Vector3 position = mVisibleObject->_getDerivedPosition();
   Ogre::Quaternion quaternion = mVisibleObject->_getDerivedOrientation();
   mPosition.setOrigin(btVector3(position.x,position.y,position.z));
   mPosition.setRotation(btQuaternion(quaternion.x,quaternion.y,quaternion.z,quaternion.w)); 
}

MoveableObjMotionState::~MoveableObjMotionState(void) { }

void MoveableObjMotionState::getWorldTransform(btTransform& worldTrans)const
{
   worldTrans = mPosition;
}

void MoveableObjMotionState::setWorldTransform (const btTransform& worldTrans)
{
   if(mVisibleObject)
   {
      btQuaternion quaternion = worldTrans.getRotation();
      btVector3 position = worldTrans.getOrigin();
      mVisibleObject->_setDerivedOrientation(Ogre::Quaternion(quaternion.getW(),quaternion.getX(),
         quaternion.getY(),quaternion.getZ()));
      mVisibleObject->_setDerivedPosition(Ogre::Vector3(position.x(),position.y(),position.z()));
      mPosition = worldTrans;
   }
}