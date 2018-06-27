#ifndef PROJECTILE_MANAGER_H
#define PROJECTILE_MANAGER_H

#include "../weapons/Projectile.h"

const unsigned int maxBombs = 50;
const unsigned int maxMisils = 50;

class ProjectileManager {

	public:
		static ProjectileManager* instance;
		Projectile bombs[maxBombs];
		Projectile misils[maxMisils];

		ProjectileManager();
		~ProjectileManager();

		void createProjectile(const Matrix44 transform, Vector3 velocity, std::string type, Airplane* owner, int damage);

		void render();
		void update(float deltaTime);

	private:

		void renderBombs();
		void renderMisils();

		void updateBombs(float deltaTime);
		void updateMisils(float deltaTime);

};

#endif
