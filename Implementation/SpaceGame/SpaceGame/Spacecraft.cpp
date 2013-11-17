#include "Spacecraft.h"

Spacecraft::Spacecraft(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, Ogre::Radian roll, Ogre::Radian pitch, Ogre::Radian yaw,
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

void Spacecraft::pitch(Ogre::Radian amount)
{
	if(amount > PITCH_RATE)
	{
		getSceneNode()->pitch(PITCH_RATE);
	} else 
	{
		getSceneNode()->pitch(amount);
	}
}

void Spacecraft::roll(Ogre::Radian amount)
{
	if(amount > ROLL_RATE)
	{
		getSceneNode()->roll(ROLL_RATE);
	} else 
	{
		getSceneNode()->roll(amount);
	}
}

void Spacecraft::yaw(Ogre::Radian amount)
{
	if(amount > YAW_RATE)
	{
		getSceneNode()->yaw(YAW_RATE);
	} else 
	{
		getSceneNode()->yaw(amount);
	}
}

void Spacecraft::accelerate(float amount)
{
	if(amount > MAX_ACCELERATION)
	{
		velocityVector.z += MAX_ACCELERATION;
	} else 
	{
		velocityVector.z += amount;
	}
}

void Spacecraft::deccelerate(float amount)
{
	if(amount > DECCELERATION_RATE)
	{
		velocityVector.z -= DECCELERATION_RATE;
	} else 
	{
		velocityVector.z -= amount;
	}
}

void Spacecraft::translateX(float amount)
{
	if(amount > TRANSLATE_RATE)
	{
		velocityVector.x += TRANSLATE_RATE;
	} else 
	{
		velocityVector.x += amount;
	}
}

void Spacecraft::translateY(float amount)
{
	if(amount > TRANSLATE_RATE)
	{
		velocityVector.y += TRANSLATE_RATE;
	} else 
	{
		velocityVector.y += amount;
	}
}

void Spacecraft::fireWeaponPrimary()
{
	//TODO
}

void Spacecraft::fireWeaponSecondary()
{
	//TODO
}