/*
 * Player.h class 
 * Author : Alex Flight
 * Version : 1.0
 * Date : 17/11/2013
 */
#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include "Engine.h"

class Player : public OIS::KeyListener, public OIS::MouseListener
{
public:
	Player();
	~Player();
private:
	void Pitch();
	void Roll();
	void Yaw();
	void Accelerate();
	void TranslateX();
	void TranslateY();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
};

#endif