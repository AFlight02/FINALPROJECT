#include "Celestial.h"


Celestial::Celestial(Ogre::String entName, Ogre::String meshName, bool isDestroyable)
	: GameEntity(entName, meshName, isDestroyable)
{
}


Celestial::~Celestial()
{
}

void Celestial::generateSphere()
{
	entity = sceneManager->createEntity("CubeSphere", Ogre::SceneManager::PrefabType::PT_CUBE);
	sceneNode->attachObject(entity);
}
