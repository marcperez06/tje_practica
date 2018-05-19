#ifndef WEAPON_H
#define WEAPON_H

#include <vector>

#include "Bullet.h"
//#include "EntityMesh.h"

class Entity;
class Airplane;

class Weapon {

	public:
		std::vector<Bullet> bullets;
		float bulletSpeed;
		float damage;
		Airplane* owner;
		float fireRate;
		float cooldown;
		std::string type;
		//EntityMesh* mesh;

		Weapon(Airplane* owner, std::string type);
		void initMisil();

		virtual void shoot();
		virtual void render();
		virtual void update(float deltaTime);
};

#endif
