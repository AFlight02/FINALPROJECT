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
	void updateAIState(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient);
	void seek(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient);
	void flee(Ogre::Vector3 targetPos, Ogre::Quaternion targetOrient);

protected:
	int currState;
};

