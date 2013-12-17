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
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_UP))
		currPitch += Ogre::Radian(6.05f);
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_DOWN))
		currPitch -= Ogre::Radian(0.05f);
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_LEFT))
		currRoll -= Ogre::Radian(0.05f);
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_RIGHT))
		currRoll += Ogre::Radian(0.05f);
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_Q))
		currYaw -= Ogre::Radian(0.05f);
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_E))
		currYaw += Ogre::Radian(0.05f);
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_W))
		currTranslateX += 0.05f;
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_S))
		currTranslateX -= 0.05f;
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_A))
		currTranslateY -= 0.05f;
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		currAcceleration += 0.05f;
	if(Engine::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_LCONTROL))
		currAcceleration -= 0.05f;
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
	pitch(currPitch*timeSinceLastFrame);
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

	if(shields < MAX_SHIELDS)
		shields += SHIELD_RECHARGE_RATE*timeSinceLastFrame;
}
