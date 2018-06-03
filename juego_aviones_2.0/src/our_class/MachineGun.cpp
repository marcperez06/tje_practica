#include "MachineGun.h"
#include "Airplane.h"
#include "BulletManager.h"

MachineGun::MachineGun(Airplane* owner, std::string type) : Weapon(owner, type) {}

void MachineGun::shoot() {
	if (this->cooldown < 0) {

		Matrix44 modelMatrix = this->owner->getGlobalMatrix();
		Vector3 leftPos = modelMatrix * Vector3(-2, 0, -2);
		Vector3 rigthPos = modelMatrix * Vector3(2, 0, -2);
		Vector3 velocity = modelMatrix.rotateVector(Vector3(0, 0, -1));
		velocity = velocity * this->owner->speed * this->bulletSpeed;

		BulletManager::instance->createBullet(leftPos, velocity, "machineGun", this->owner, this->damage);
		BulletManager::instance->createBullet(rigthPos, velocity, "machineGun", this->owner, this->damage);

		this->cooldown = 0.50;
	}
}

void MachineGun::update(float deltaTime) {
	Weapon::update(deltaTime);
}