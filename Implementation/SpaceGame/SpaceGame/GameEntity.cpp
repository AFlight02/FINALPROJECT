#include "GameEntity.h"

GameEntity::GameEntity(Ogre::String eName, Ogre::String mName, bool isDestroyable)
{
	destroyable = isDestroyable;
	entityName = eName;
	meshName = mName;
	alive = true;
	sceneManager = Engine::getSingletonPtr()->mSceneManager;
	entity = sceneManager->createEntity(entityName, meshName);
	sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(entityName+"_Node");
	sceneNode->attachObject(entity);
	//sceneNode->setPosition(Ogre::Vector3(0, 0, -25));
}


GameEntity::~GameEntity()
{
	entity->detachFromParent();
	sceneNode->removeAndDestroyAllChildren();
}

Ogre::Vector3 GameEntity::getPosition()
{
	return sceneNode->getPosition();
}

Ogre::SceneNode* GameEntity::getSceneNode()
{
	return sceneNode;
}

void GameEntity::setPosition(Ogre::Vector3 pos)
{
	sceneNode->setPosition(pos);
}

bool GameEntity::destroy()
{
	if(destroyable)
	{
		alive = false;
	} else
	{
		Engine::getSingletonPtr()->mLog->logMessage(this->entityName+"_Entity is not DESTROYABLE");
		return false;
	}
	return true;
}

bool GameEntity::isAlive()
{
	return alive;
}
