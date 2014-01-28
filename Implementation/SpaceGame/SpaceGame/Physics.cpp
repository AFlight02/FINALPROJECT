#include "Physics.h"


Physics::Physics()
{
	bulBroadphase = new btDbvtBroadphase();
	bulCollisionConfiguration = new btDefaultCollisionConfiguration();
	bulDispatcher = new btCollisionDispatcher(bulCollisionConfiguration);
	bulSolver = new btSequentialImpulseConstraintSolver;
	bulDynamicsWorld = new btDiscreteDynamicsWorld(bulDispatcher, bulBroadphase, bulSolver, bulCollisionConfiguration);
	bulDynamicsWorld->setGravity(btVector3(0,0,0));
	bulDebugDrawer = new BtOgre::DebugDrawer(Engine::getSingletonPtr()->mSceneManager->getRootSceneNode(), bulDynamicsWorld);
}


Physics::~Physics()
{
	//delete bulDynamicsWorld;
    //delete bulSolver;
    //delete bulDispatcher;
    //delete bulCollisionConfiguration;
    //delete bulBroadphase;
}

void Physics::addRigidBody(btRigidBody* body)
{
	bulDynamicsWorld->addRigidBody(body);
}

void Physics::stepSimulation(double timeSinceLastFrame, double subSteps)
{
	bulDynamicsWorld->stepSimulation(timeSinceLastFrame, subSteps);
	bulDebugDrawer->step();
}

void Physics::setupDebugDrawer()
{
	bulDynamicsWorld->setDebugDrawer(bulDebugDrawer);
}