#include "Enemy.h"

Enemy::Enemy() : Spacecraft("DEFAULT_Enemy", "enemyShip1.mesh", true, 2000, 400, 25, 5, 5, 5, 25, 15)
{
	currState = -1;
}

Enemy::Enemy(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, float roll, float pitch, float yaw,
					   float decceleration, float translate)
		: Spacecraft(entName, meshName, isDestroyable, objMass, maxVel, maxAccel, roll, pitch, yaw, decceleration, translate)
{
	currState = -1; //DEFAULT STATE
}

Enemy::~Enemy()
{
}

int Enemy::getAIState()
{
	return currState;
}

void Enemy::updateAIState(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient)
{
	if(hull < (MAX_HULL * .25f))
	{
		currState = 0; //FLEE STATE
		flee(targetPos, targetOrient);
	} else
	{
		currState = 1; //SEEK STATE
		seek(targetPos, targetOrient);
	}
}

void Enemy::seek(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient)
{
	Ogre::Vector3 currPos = sceneNode->getPosition();
	Ogre::Quaternion currOrient = sceneNode->getOrientation();
	Ogre::Vector3 vectorToTarget = targetPos - currPos;
	currPos.normalise();
	currOrient.normalise();

	Ogre::Vector3 heading = currOrient * currPos;
	Ogre::Vector3 targetHeading = targetOrient * targetPos;
	heading.normalise();
	targetHeading.normalise();

	Ogre::Vector3 currVectorOrientation = currOrient.Inverse() * Ogre::Vector3::UNIT_Y;
	Ogre::Vector3 targetVectorOrientation = targetOrient * Ogre::Vector3::UNIT_Y;

	Ogre::Vector3 torque = currOrient.Inverse() * vectorToTarget;
	torque.normalise();

	pitch(torque.y);
	yaw(torque.x);
	roll(targetVectorOrientation.getRotationTo(currVectorOrientation).getRoll().valueRadians());
	
	if(sceneNode->getPosition().squaredDistance(targetPos) > 100)
	{
		thrust(1.0f);
	} else if (sceneNode->getPosition().squaredDistance(targetPos) <= 100) 
	{
		thrust(-1.0f);
	}
}

void Enemy::flee(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient)
{
	Ogre::Vector3 currPos = sceneNode->getPosition();
	Ogre::Quaternion currOrient = sceneNode->getOrientation();
	Ogre::Vector3 vectorToTarget = targetPos - currPos;
	currPos.normalise();
	currOrient.normalise();

	Ogre::Vector3 heading = currOrient * currPos;
	Ogre::Vector3 targetHeading = targetOrient * targetPos;
	heading.normalise();
	targetHeading.normalise();

	Ogre::Vector3 currVectorOrientation = currOrient.Inverse() * Ogre::Vector3::UNIT_Y;
	Ogre::Vector3 targetVectorOrientation = targetOrient * Ogre::Vector3::UNIT_Y;

	Ogre::Vector3 torque = currOrient * vectorToTarget;
	torque.normalise();

	pitch(torque.y);
	yaw(torque.x);
	roll(targetVectorOrientation.getRotationTo(currVectorOrientation).getRoll().valueRadians());
	
	if(sceneNode->getPosition().squaredDistance(targetPos) > 100)
	{
		thrust(1.0f);
	} else if (sceneNode->getPosition().squaredDistance(targetPos) <= 100) 
	{
		thrust(-1.0f);
	}
}