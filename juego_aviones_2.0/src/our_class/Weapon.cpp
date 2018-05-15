#include "Weapon.h"
#include "Factory.h"
#include "World.h"

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
	this->cooldown = 5;
	this->fireRate = 1;
	//this->mesh = Factory::buildMisil(World::instance->player->getGlobalMatrix() * Vector3(0, -1, 0));
}

void Weapon::initMachineGun() {
	this->damage = 60;
	this->bulletSpeed = 7;
	this->cooldown = 2;
	this->fireRate = 10;
	//this->mesh = Factory::buildMisil(World::instance->player->getGlobalMatrix() * Vector3(0, -1, 0));
}

void Weapon::shoot(Matrix44 parentTransform) {

	Vector3 initialPos = parentTransform * Vector3(0, -3, 0);
	Vector3 velocity = Vector3(0, 0, bulletSpeed * -1);
	if (this->cooldown < 0) {
		this->bullets.push_back(Factory::buildBullet(initialPos, velocity, this->damage, 2, this->owner, this->type));
		this->cooldown = 2;
	}
}

void Weapon::render() {
	/*for (int i = 0; i < this->bullets.size(); i++) {
		this->bullets[i]->render();
	}*/
	//mesh->render(World::instance->currentCamera);
}

void Weapon::update(float deltaTime) {
	this->cooldown -= deltaTime;
	/*for (int i = 0; i < this->bullets.size(); i++) {
		this->bullets[i]->update();
	}*/
}