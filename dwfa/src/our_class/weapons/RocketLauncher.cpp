#include "RocketLauncher.h"
#include "../entities/Airplane.h"
#include "../Factory.h"
#include "../../camera.h"
#include "../World.h"
#include "../managers/ProjectileManager.h"
#include "../managers/SoundManager.h"

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

		SoundManager::reproduceSound("misil_shoot.wav");
	} else {
		SoundManager::reproduceSound("no_ammounition_original.wav");
	}
}