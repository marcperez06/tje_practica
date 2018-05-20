#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "Bullet.h"

const unsigned int maxBullets = 1024;

class BulletManager {
	public:
		static BulletManager* instance;
		Bullet bullets[maxBullets];

		BulletManager();

		void createBullet(Vector3 pos, Vector3 velocity, std::string type, Airplane* owner);

		void render();
		void update(float deltaTime);

};

#endif
