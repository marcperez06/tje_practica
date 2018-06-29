#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../framework.h"
#include "Bullet.h"

class Airplane;

class Projectile : public Bullet{
	public:
		Matrix44 transform;
		Projectile();
		void setProperties(const Matrix44 transform, const Vector3 velocity, float timeToLive, std::string type, Airplane* owner, int damage);

};

#endif
