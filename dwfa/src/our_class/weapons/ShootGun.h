#ifndef SHOOT_GUN_H
#define SHOOT_GUN_H

#include "Weapon.h"

class Airplane;

class ShootGun : public Weapon {

public:

	ShootGun(Airplane* owner, char type);

	virtual void shoot();
};


#endif