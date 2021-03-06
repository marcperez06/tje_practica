#include "MachineGun.h"
#include "../entities/Airplane.h"
#include "../managers/BulletManager.h"
#include "../managers/SoundManager.h"

MachineGun::MachineGun(Airplane* owner, char type) : Weapon(owner, type) {}

void MachineGun::shoot() {
	if (this->cooldown < 0) {

		Matrix44 modelMatrix = this->owner->getGlobalMatrix();
		Vector3 leftPos = modelMatrix * Vector3(-2, 0, -2);
		Vector3 rigthPos = modelMatrix * Vector3(2, 0, -2);
		Vector3 velocity = modelMatrix.rotateVector(Vector3(0, 0, -1));
		velocity = velocity * this->owner->speed * this->bulletSpeed;

		BulletManager::instance->createBullet(leftPos, velocity, "machineGun", this->owner, this->damage);
		BulletManager::instance->createBullet(rigthPos, velocity, "machineGun", this->owner, this->damage);

		this->cooldown = 0.25;
		SoundManager::reproduceSound("normal_shoot.wav");
	}
}