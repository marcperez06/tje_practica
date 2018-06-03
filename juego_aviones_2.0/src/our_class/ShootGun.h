#ifndef SHOOT_GUN_H
#define SHOOT_GUN_H

#include "Weapon.h"

class Airplane;

class ShootGun : public Weapon {

public:

	ShootGun(Airplane* owner, std::string type);

	virtual void shoot();
	virtual void update(float deltaTime);
};


#endif