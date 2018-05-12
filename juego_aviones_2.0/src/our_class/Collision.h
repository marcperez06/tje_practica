#ifndef COLLISION_H
#define COLLISION_H

#include "../framework.h"

class Collision {
	public:
		Vector3 origin;
		Vector3 direction;
		Vector3 collisionPoint;
		Vector3 normalPoint;
		bool haveCollision;

		Collision();
};


#endif
