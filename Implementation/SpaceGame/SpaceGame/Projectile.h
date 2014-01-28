#pragma once
#include "Moveableobject.h"

class Projectile : public MoveableObject
{
public:
	Projectile(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass);
	~Projectile();
};

