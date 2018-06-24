#ifndef DROP_BOMB_H
#define DROP_BOMB_H

#include "Weapon.h"
#include "Projectile.h"

class Airplane;

class DropBomb : public Weapon {

public:

	DropBomb(Airplane* owner, char type);

	virtual void shoot();

};


#endif