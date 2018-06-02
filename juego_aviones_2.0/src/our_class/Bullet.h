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
		void setProperties(const Vector3 pos, const Vector3 velocity, float timeToLive, std::string type, Airplane* owner, int damage);

};

#endif // !BULLET_H

