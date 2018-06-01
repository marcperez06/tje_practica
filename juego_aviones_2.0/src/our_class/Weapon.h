#ifndef WEAPON_H
#define WEAPON_H

#include <vector>

#include "Bullet.h"


class Entity;
class Airplane;

class Weapon {

	public:
		float bulletSpeed;
		float damage;
		Airplane* owner;
		float fireRate;
		float cooldown;
		std::string type;
		

		Weapon(Airplane* owner, std::string type);

		virtual void shoot();
		virtual void render();
		virtual void update(float deltaTime);
};

#endif
