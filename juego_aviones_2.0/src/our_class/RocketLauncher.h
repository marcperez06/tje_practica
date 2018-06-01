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

	RocketLauncher(Airplane* owner, std::string type);

	virtual void shoot();
	virtual void render();
	virtual void update(float deltaTime);

};


#endif