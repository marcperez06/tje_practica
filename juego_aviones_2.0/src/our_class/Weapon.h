#ifndef WEAPON_H
#define WEAPON_H

#include <vector>

#include "Bullet.h"


class Entity;
class Airplane;

enum WeaponType {
	MACHINE_GUN,
	SHOOT_GUN,
	ROCKET_LAUNCHER,
	DROP_BOMB
};

class Weapon {

	public:
		float bulletSpeed;
		float damage;
		Airplane* owner;
		float fireRate;
		float cooldown;
		int ammounition;

		char type;
		

		Weapon(Airplane* owner, char type);

		virtual void shoot();
		virtual void render();
		virtual void update(float deltaTime);
		
		void addAmmounition(int ammo);
};

#endif
