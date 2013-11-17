/*
 * GameEntity.h class 
 * Author : Alex Flight
 * Version : 1.0
 * Date : 17/11/2013
 */
#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#pragma once

#include "Engine.h"

class GameEntity
{
public:
	GameEntity(Ogre::String entName, Ogre::String meshName, bool isDestroyable);
	~GameEntity();

	Ogre::Vector3 getPosition();
	Ogre::SceneNode* getSceneNode();
	void setPosition(Ogre::Vector3 pos);
	bool destroy();
	bool isAlive();

protected:
	Ogre::SceneManager* sceneManager;
	Ogre::Entity* entity;
	Ogre::SceneNode* sceneNode;
	Ogre::Vector3 position;
	Ogre::String entityName;
	Ogre::String meshName;

    bool destroyable;
    bool alive;
};

#endif