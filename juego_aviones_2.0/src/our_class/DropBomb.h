#ifndef DROP_BOMB_H
#define DROP_BOMB_H

#include "Weapon.h"
#include "Bullet.h"

const unsigned int maxBombs = 5;

class Airplane;

class DropBomb : public Weapon {

public:

	Bullet bombs[maxBombs];

	DropBomb(Airplane* owner, std::string type);

	virtual void shoot();
	virtual void render();
	virtual void update(float deltaTime);

};


#endif