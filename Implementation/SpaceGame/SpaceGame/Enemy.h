#pragma once
#include "spacecraft.h"
class Enemy : public Spacecraft
{
public:
	Enemy();
	Enemy(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, float roll, float pitch, float yaw,
		float decceleration, float translate);
	~Enemy();

	int getAIState();
	int getPatrolPoint();
	int updateAIState(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient);
	int seek(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient);
	int flee(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient);
	int patrol(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient);

	Ogre::SceneNode* waypoints[4];

protected:
	int currState;
	int currentWaypoint;
};

