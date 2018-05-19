#ifndef DROP_BOMB_H
#define DROP_BOMB_H

#include "Weapon.h"

class Airplane;

class DropBomb : public Weapon {

public:

	DropBomb(Airplane* owner, std::string type);

	virtual void shoot();
	virtual void render();
	virtual void update(float deltaTime);

	void testStaticCollisions();
};


#endif