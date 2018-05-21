#ifndef MISIL_H
#define MISIL_H

#include "Weapon.h"
#include "Bullet.h"

const unsigned int maxMisil = 5;

class Airplane;
class EntityMesh;

class Misil : public Weapon {

public:

	Bullet misils[maxMisil];
	EntityMesh* meshMisil;

	Misil(Airplane* owner, std::string type);

	virtual void shoot();
	virtual void render();
	virtual void update(float deltaTime);

};


#endif