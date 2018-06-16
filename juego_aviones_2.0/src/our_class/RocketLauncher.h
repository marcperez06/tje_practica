#ifndef MISIL_H
#define MISIL_H

#include "Weapon.h"
#include "Projectile.h"

const unsigned int maxMisil = 5;

class Airplane;
class EntityMesh;

class RocketLauncher : public Weapon {

public:

	Projectile misils[maxMisil];

	RocketLauncher(Airplane* owner, char type);

	virtual void shoot();
};


#endif