#include "Physics.h"


Physics::Physics()
{
	bulBroadphase = new btDbvtBroadphase();
	bulCollisionConfiguration = new btDefaultCollisionConfiguration();
	bulDispatcher = new btCollisionDispatcher(bulCollisionConfiguration);
	bulSolver = new btSequentialImpulseConstraintSolver;
	bulDynamicsWorld = new btDiscreteDynamicsWorld(bulDispatcher, bulBroadphase, bulSolver, bulCollisionConfiguration);
	bulDynamicsWorld->setGravity(btVector3(0,0,0));
}


Physics::~Physics()
{
}

void Physics::addRigidBody(btRigidBody* body)
{
	bulDynamicsWorld->addRigidBody(body);
}

void Physics::stepSimulation(double timeSinceLastFrame, double subSteps)
{
	bulDynamicsWorld->stepSimulation(timeSinceLastFrame, subSteps);
}
