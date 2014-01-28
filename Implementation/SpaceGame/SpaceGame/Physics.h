#pragma once
#include "Engine.h"
class Physics
{
public:
	Physics();
	~Physics();

	void addRigidBody(btRigidBody* body);
	void stepSimulation(double timeSinceLastFrame, double subSteps);

	void setupDebugDrawer();

protected:
	// Bullet Resources
	btBroadphaseInterface* bulBroadphase;
	btDefaultCollisionConfiguration* bulCollisionConfiguration;
	btCollisionDispatcher* bulDispatcher;
	btSequentialImpulseConstraintSolver* bulSolver;
	btDiscreteDynamicsWorld* bulDynamicsWorld;

	BtOgre::DebugDrawer* bulDebugDrawer;
};

