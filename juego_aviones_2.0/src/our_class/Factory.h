#ifndef FACTORY_H
#define FACTORY_H

#include "Airplane.h"
#include "Bullet.h"
#include "../framework.h"

class Factory {

	public:

		static Airplane* buildAirplane(const Vector3 initialPos, float speed);
		static Bullet* buildBullet(const Vector3 pos, const Vector3 velocity, float power, float timeToLive, int author, std::string type);

};

#endif // !FACTORY_H
