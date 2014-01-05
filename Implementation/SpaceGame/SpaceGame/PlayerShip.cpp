#include "PlayerShip.h"

PlayerShip::PlayerShip(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, Ogre::Radian roll, Ogre::Radian pitch, Ogre::Radian yaw,
					   float decceleration, float translate)
		: Spacecraft(entName, meshName, isDestroyable, objMass, maxVel, maxAccel, roll, pitch, yaw, decceleration, translate)
{
}


PlayerShip::~PlayerShip()
{
}

void PlayerShip::setupPhysics()
{
	btScalar mass = 5000;
	btVector3 shipInertia;
	shipShape = new btSphereShape(1);
	shipShape->calculateLocalInertia(mass,shipInertia);
	
	motionState = new MoveableObjMotionState(sceneNode);

	btRigidBody::btRigidBodyConstructionInfo shipRigidBodyCI(mass,motionState,shipShape,shipInertia);
	shipRigidBody = new btRigidBody(shipRigidBodyCI);
	shipRigidBody->setDamping(0.1,0.1);
	shipRigidBody->activate();
	currPitch = 0;
	currRoll = 0;
	currYaw = 0;
	currTranslateX = 0;
	currTranslateY = 0;
	currTranslateZ = 0;
	rotation = btVector3(0,0,0);
	translation = btVector3(0,0,0);
}

btRigidBody* PlayerShip::getRigidBody()
{
	return shipRigidBody;
}

void PlayerShip::targetObject()
{
	//TODO
}

void PlayerShip::toggleAdvFlightMode()
{
	//TODO
}

bool PlayerShip::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

bool PlayerShip::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	Engine::getSingletonPtr()->keyPressed(keyEventRef);
	return true;
}

bool PlayerShip::mouseMoved(const OIS::MouseEvent &arg)
{
	return true;
}

bool PlayerShip::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}

bool PlayerShip::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}

void PlayerShip::update(double timeSinceLastFrame)
{
	rotation += btVector3(currPitch, currYaw, currRoll);
	translation += btVector3(currTranslateX, currTranslateY, currTranslateZ);
	//shipRigidBody->applyTorque(btVector3(currPitch,currYaw, currRoll)*timeSinceLastFrame);

	//shipRigidBody->applyImpulse(btVector3(currTranslateX,currTranslateY,currTranslateZ), shipRigidBody->getCenterOfMassTransform().getOrigin());
	//pitch(currPitch*timeSinceLastFrame);
	//roll(currRoll*timeSinceLastFrame);
	//yaw(currYaw*timeSinceLastFrame);
	//translateX(currTranslateX*timeSinceLastFrame);
	//translateY(currTranslateY*timeSinceLastFrame);
	//accelerate(currAcceleration*timeSinceLastFrame);

	//currPitch -= Ogre::Radian(0.05f*timeSinceLastFrame);
	//currRoll -= Ogre::Radian(0.05f*timeSinceLastFrame);
	//currYaw -= Ogre::Radian(0.05f*timeSinceLastFrame);
	//currTranslateX -= currTranslateX*timeSinceLastFrame;
	//currTranslateY -= currTranslateY*timeSinceLastFrame;
	//currAcceleration -= currAcceleration*timeSinceLastFrame;

//	if(currPitch > 0 || currYaw > 0 || currRoll > 0 || currTranslateX > 0 || currTranslateY > 0 || currTranslateZ)
//		currPitch,currYaw,currRoll,currTranslateX,currTranslateY,currTranslateZ -= timeSinceLastFrame;

	if(rotation.x() > 0)
		rotation.setX(rotation.x()-timeSinceLastFrame);
	if(rotation.x() < 0)
		rotation.setX(rotation.x()+timeSinceLastFrame);
	if(rotation.y() > 0)
		rotation.setY(rotation.y()-timeSinceLastFrame);
	if(rotation.y() < 0)
		rotation.setY(rotation.y()+timeSinceLastFrame);
	if(rotation.z() > 0)
		rotation.setZ(rotation.z()-timeSinceLastFrame);
	if(rotation.z() < 0)
		rotation.setZ(rotation.z()+timeSinceLastFrame);

	if(translation.x() > 0)
		translation.setX(translation.x()-timeSinceLastFrame);
	if(translation.x() < 0)
		translation.setX(translation.x()+timeSinceLastFrame);
	if(translation.y() > 0)
		translation.setY(translation.y()-timeSinceLastFrame);
	if(translation.y() < 0)
		translation.setY(translation.y()+timeSinceLastFrame);
	if(translation.z() > 0)
		translation.setZ(translation.z()-timeSinceLastFrame);
	if(translation.z() < 0)
		translation.setZ(translation.z()+timeSinceLastFrame);

	if(shields < MAX_SHIELDS)
		shields += SHIELD_RECHARGE_RATE*timeSinceLastFrame;
}
