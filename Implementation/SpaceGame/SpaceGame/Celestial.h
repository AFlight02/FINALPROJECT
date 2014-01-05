#pragma once
#include "GameEntity.h"

class Celestial : public GameEntity
{
public:
	Celestial(Ogre::String entName, Ogre::String meshName, bool isDestroyable);
	~Celestial();

	void generateSphere();

protected:
};

