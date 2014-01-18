#include "Spacecraft.h"

Spacecraft::Spacecraft(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, float roll, float pitch, float yaw,
		float decceleration, float translate)
		: MoveableObject(entName, meshName, isDestroyable, objMass)
{
	MAX_ACCELERATION = maxAccel;
	MAX_VELOCITY = maxVel;
	ROLL_RATE = roll;
	PITCH_RATE = pitch;
	YAW_RATE = yaw;
	DECCELERATION_RATE = decceleration;
	TRANSLATE_RATE = translate;
}


Spacecraft::~Spacecraft()
{
}

float Spacecraft::getShields()
{
	return shields;
}

float Spacecraft::getHull()
{
	return hull;
}

void Spacecraft::applyDamage(float amount)
{
	if(amount > shields)
	{
		float bleedThrough = amount - shields;
		shields = 0;
		hull -= bleedThrough;
	} else 
	{
		shields -= amount;
	}
}

void Spacecraft::pitch(float amount)
{
	currPitch = amount * PITCH_RATE;
}

void Spacecraft::roll(float amount)
{
	currRoll = amount * ROLL_RATE;
}

void Spacecraft::yaw(float amount)
{
	currYaw = amount * YAW_RATE;
}

void Spacecraft::thrust(float amount)
{
	currTranslateZ = amount * MAX_ACCELERATION;
}

void Spacecraft::translateX(float amount)
{
	currTranslateX = amount * TRANSLATE_RATE;
}

void Spacecraft::translateY(float amount)
{
	currTranslateY = amount * TRANSLATE_RATE;
}

void Spacecraft::fireWeaponPrimary()
{
	//TODO
}

void Spacecraft::fireWeaponSecondary()
{
	//TODO
}

btRigidBody* Spacecraft::getRigidBody()
{
	return shipRigidBody;
}

btVector3 Spacecraft::getTranslationVector()
{
	return translation;
}
btVector3 Spacecraft::getRotationVector()
{
	return rotation;
}

void Spacecraft::setupPhysics()
{
        btScalar bMass = MASS;
        btVector3 shipInertia;
        shipShape = new btSphereShape(1);
        shipShape->calculateLocalInertia(bMass,shipInertia);
        
        motionState = new MoveableObjMotionState(sceneNode);

        btRigidBody::btRigidBodyConstructionInfo shipRigidBodyCI(bMass,motionState,shipShape,shipInertia);
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

void Spacecraft::update(double timeSinceLastFrame)
{
	rotation += btVector3(currPitch, currYaw, currRoll);
	translation += btVector3(currTranslateX, currTranslateY, currTranslateZ);

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
}