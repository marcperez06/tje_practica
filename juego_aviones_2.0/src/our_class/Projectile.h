#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../framework.h"
#include "Bullet.h"

class Airplane;

class Projectile : public Bullet{
	public:
		Matrix44 transform;
		Projectile();

};

#endif
