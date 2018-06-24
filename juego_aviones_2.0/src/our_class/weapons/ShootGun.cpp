#include "ShootGun.h"
#include "../entities/Airplane.h"
#include "../managers/BulletManager.h"
#include "../managers/SoundManager.h"

ShootGun::ShootGun(Airplane* owner, char type) : Weapon(owner, type) {}

void ShootGun::shoot() {
	if (this->cooldown < 0) {

		Matrix44 modelMatrix = this->owner->getGlobalMatrix();
		Vector3 origin = modelMatrix * Vector3(0, 0, -2);

		for (int i = -2; i <= 2; i++) {

			for (int j = -2; j <= 2; j++) {

				Vector3 bulletGoal = modelMatrix * Vector3(i * 0.25, j * 0.25, -7);
				Vector3 direction = (bulletGoal - origin).normalize();

				Vector3 velocity = direction;//modelMatrix.rotateVector(Vector3(0, 0, -1));
				velocity = velocity * this->owner->speed * this->bulletSpeed;

				BulletManager::instance->createBullet(origin, velocity, "shootGun", this->owner, this->damage);

			}
		}

		this->cooldown = 1.50;
		SoundManager::reproduceSound("shootgun_shoot.wav");
	}
}