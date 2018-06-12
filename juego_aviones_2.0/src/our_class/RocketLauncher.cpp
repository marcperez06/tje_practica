#include "RocketLauncher.h"
#include "Airplane.h"
#include "Factory.h"
#include "../Camera.h"
#include "World.h"
#include "ProjectileManager.h"

RocketLauncher::RocketLauncher(Airplane* owner, char type) : Weapon(owner, type) {
	memset(&this->misils, 0, sizeof(misils));
}

void RocketLauncher::shoot() {
	if (this->cooldown < 0 && this->ammounition > 0) {

		Matrix44 misilTransform = this->owner->getGlobalMatrix();
		misilTransform.translate(0, -2, 0);
		misilTransform.rotate(180 * DEG2RAD, Vector3(0, 1, 0));

		Vector3 velocity = Vector3(0, 0, 1);

		velocity = velocity * this->bulletSpeed;

		ProjectileManager::instance->createProjectile(misilTransform, velocity, "rocketLauncher", this->owner, this->damage);

		this->cooldown = 1;
		this->ammounition -= 1;
	}
}

void RocketLauncher::render() {

	/*
	Mesh mesh1;
	for (int i = 0; i < maxMisil; i++) {
		Bullet& misil = this->misils[i];
		if (misil.timeToLive > 0) {
			mesh1.vertices.push_back(misil.position);
			mesh1.colors.push_back(Vector4(1, 0, 0, 1));
		}
	}

	if (mesh1.vertices.size() > 0) {
		glPointSize(10);
		mesh1.renderFixedPipeline(GL_POINTS);
	}
	*/
	

}

/*
void RocketLauncher::update(float deltaTime) {
	Weapon::update(deltaTime);
}
*/