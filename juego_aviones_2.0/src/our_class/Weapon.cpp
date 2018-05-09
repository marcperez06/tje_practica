#include "Weapon.h"
#include "Factory.h"

Weapon::Weapon(int ownser, std::string type) {

	this->owner = owner;
	this->type = type;

	if (type.compare("Misil") == 0) {
		initMisil();
	} else if (type.compare("MachineGun") == 0) {
		initMachineGun();
	}
}

void Weapon::initMisil() {
	this->damage = 100;
	this->bulletSpeed = 10;
	this->cooldown = 2;
	this->fireRate = 1;
}

void Weapon::initMachineGun() {
	this->damage = 100;
	this->bulletSpeed = 10;
	this->cooldown = 2;
	this->fireRate = 1;
}

void Weapon::shoot(Matrix44 parentTransform) {

	Vector3 initialPos = parentTransform * Vector3(0, -3, 0);
	Vector3 velocity = Vector3(0, 0, bulletSpeed * -1);
	if (this->bullets.size() == 0)
		this->bullets.push_back(Factory::buildBullet(initialPos, velocity, this->damage, 2, this->owner, this->type));
}

void Weapon::render() {
	for (int i = 0; i < this->bullets.size(); i++) {
		this->bullets[i]->render();
	}
}

void Weapon::update() {
	for (int i = 0; i < this->bullets.size(); i++) {
		this->bullets[i]->update();
	}
}