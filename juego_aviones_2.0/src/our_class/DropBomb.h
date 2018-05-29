#ifndef DROP_BOMB_H
#define DROP_BOMB_H

#include "Weapon.h"
#include "Projectile.h"

const unsigned int maxBombs = 5;

class Airplane;

class DropBomb : public Weapon {

public:

	Projectile bombs[maxBombs];

	DropBomb(Airplane* owner, std::string type);

	virtual void shoot();
	virtual void render();
	virtual void update(float deltaTime);

};


#endif