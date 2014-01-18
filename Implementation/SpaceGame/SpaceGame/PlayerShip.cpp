#include "PlayerShip.h"

PlayerShip::PlayerShip(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass, float maxVel, float maxAccel, float roll, float pitch, float yaw,
					   float decceleration, float translate)
		: Spacecraft(entName, meshName, isDestroyable, objMass, maxVel, maxAccel, roll, pitch, yaw, decceleration, translate)
{
}


PlayerShip::~PlayerShip()
{
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
