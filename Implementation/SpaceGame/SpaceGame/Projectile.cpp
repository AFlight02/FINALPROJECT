#include "Projectile.h"


Projectile::Projectile(Ogre::String entName, Ogre::String meshName, bool isDestroyable, float objMass)
	: MoveableObject(entName, meshName, isDestroyable, objMass)
{
}


Projectile::~Projectile()
{
}
