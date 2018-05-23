#ifndef BULLET_H
#define BULLET_H

#include "../framework.h"
//#include "Airplane.h"

class Airplane;

class Bullet {
	public:
		Vector3 position;
		Vector3 lastPosition;
		Vector3 velocity;
		float timeToLive;
		Airplane* owner;
		std::string type;

		int damage;

		Bullet();

};

#endif // !BULLET_H

