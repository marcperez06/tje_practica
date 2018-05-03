#ifndef BULLET_H
#define BULLET_H

#include "../framework.h"

class Bullet {
	public:
		Vector3 position;
		Vector3 lastPosition;
		Vector3 velocity;
		float timeToLive;
		float power;
		int author; // uuid de la entidad que dispara.
		std::string type;

		Bullet();

};

#endif // !BULLET_H

