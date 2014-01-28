#include "Enemy.h"

Enemy::Enemy() : Spacecraft("DEFAULT_Enemy", "enemyShip1.mesh", true, 2000, 50, 25, 5, 5, 5, 25, 15)
{
	currState = -1;
	currentWaypoint = 0;
	waypoints[0] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 1", Ogre::Vector3(100,30,-100));
	waypoints[1] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 2", Ogre::Vector3(0,300,0));
	waypoints[2] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 3", Ogre::Vector3(0,-50,-300));
	waypoints[3] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 4", Ogre::Vector3(100,100,100));
}

Enemy::Enemy(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, float roll, float pitch, float yaw,
					   float decceleration, float translate)
		: Spacecraft(entName, meshName, isDestroyable, objMass, maxVel, maxAccel, roll, pitch, yaw, decceleration, translate)
{
	currState = -1; //DEFAULT STATE
	currentWaypoint = 0;
	waypoints[0] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 1", Ogre::Vector3(100,30,-100));
	waypoints[1] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 2", Ogre::Vector3(0,300,0));
	waypoints[2] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 3", Ogre::Vector3(0,-50,-300));
	waypoints[3] = Engine::getSingletonPtr()->mSceneManager->getRootSceneNode()->createChildSceneNode("Waypoint 4", Ogre::Vector3(100,100,100));
}

Enemy::~Enemy()
{
}

int Enemy::getAIState()
{
	return currState;
}

int Enemy::getPatrolPoint()
{
	return currentWaypoint;
}

int Enemy::updateAIState(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient)
{
	int distanceToTarget = 0;
	if(hull < (MAX_HULL * .25f))
	{
		currState = 0; //FLEE STATE
		distanceToTarget = flee(targetPos, targetOrient);
	} if(sceneNode->getPosition().squaredDistance(targetPos) < 500)
	{
		currState = 1; //SEEK STATE
		distanceToTarget = seek(targetPos, targetOrient);
	} else
	{
		currState = 2; //PATROL STATE/
		distanceToTarget = patrol(waypoints[currentWaypoint]->getPosition(), waypoints[currentWaypoint]->getOrientation());
	}
	return distanceToTarget;
}

int Enemy::seek(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient)
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

	// CHEATING - MAKING ENEMY MOVEMENT WORK WITH SCENENODE MANIPULATION FOR NOW!
	/*sceneNode->pitch(Ogre::Radian(torque.y));
	sceneNode->yaw(Ogre::Radian(torque.x));
	sceneNode->roll(Ogre::Radian(targetVectorOrientation.getRotationTo(currVectorOrientation).getRoll().valueRadians()));

	if(sceneNode->getPosition().squaredDistance(targetPos) > 1000)
	{
		sceneNode->translate(Ogre::Vector3(0,0,10));
	} else if (sceneNode->getPosition().squaredDistance(targetPos) <= 500) 
	{
		sceneNode->translate(Ogre::Vector3(0,0,-10));
	} else 
	{
		sceneNode->translate(Ogre::Vector3(0,0,0));
	}*/

	// PHYSICS BASED MOVEMENT - NOT YET WORKING - COMING BACK TO IT IN LATER IMPLEMENTATIONS
	pitch(torque.y);
	yaw(torque.x);
	roll(targetVectorOrientation.getRotationTo(currVectorOrientation).getRoll().valueRadians());
	
	if(sceneNode->getPosition().squaredDistance(targetPos) > 100)
	{
		//thrust(1.0f);
	} else if (sceneNode->getPosition().squaredDistance(targetPos) <= 100) 
	{
		thrust(0.0f);
	}
	return sceneNode->getPosition().squaredDistance(targetPos);
}

int Enemy::flee(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient)
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

	// CHEATING - MAKING ENEMY MOVEMENT WORK WITH SCENENODE MANIPULATION FOR NOW!
	//sceneNode->pitch(Ogre::Radian(torque.y));
	//sceneNode->yaw(Ogre::Radian(torque.x));
	//sceneNode->roll(Ogre::Radian(targetVectorOrientation.getRotationTo(currVectorOrientation).getRoll().valueRadians()));

	//if(sceneNode->getPosition().squaredDistance(targetPos) > 100)
	//{
	//	sceneNode->translate(Ogre::Vector3(0,0,2));
	//} else if (sceneNode->getPosition().squaredDistance(targetPos) <= 100) 
	//{
	//	sceneNode->translate(Ogre::Vector3(0,0,0));
	//}

	// PHYSICS BASED MOVEMENT - NOT YET WORKING - COMING BACK TO IT IN LATER IMPLEMENTATIONS
	pitch(torque.y);
	yaw(torque.x);
	roll(targetVectorOrientation.getRotationTo(currVectorOrientation).getRoll().valueRadians());
	
	if(sceneNode->getPosition().squaredDistance(targetPos) > 100)
	{
		//thrust(1.0f);
	} else if (sceneNode->getPosition().squaredDistance(targetPos) <= 100) 
	{
		thrust(0.0f);
	}
	return sceneNode->getPosition().squaredDistance(targetPos);
}

int Enemy::patrol(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient)
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
		thrust(.6f);
	} else if (sceneNode->getPosition().squaredDistance(targetPos) <= 100) 
	{
		if(currentWaypoint <= 3)
			currentWaypoint++;
		else
			currentWaypoint = 0;
		thrust(0.0f);
	}
	return sceneNode->getPosition().squaredDistance(targetPos);
}