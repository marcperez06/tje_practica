#ifndef WEAPON_H
#define WEAPON_H

#include <vector>
#include "Bullet.h"
//#include "EntityMesh.h"

class Weapon {

	public:
		std::vector<Bullet*> bullets;
		float bulletSpeed;
		float damage;
		int owner;
		float fireRate;
		float cooldown;
		std::string type;
		//EntityMesh* mesh;

		Weapon(int owner, std::string type);
		void initMisil();
		void initMachineGun();

		void shoot(Matrix44 parentTransform);
		void render();
		void update(float deltaTime);
};

#endif
