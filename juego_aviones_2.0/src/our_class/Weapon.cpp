#include "Weapon.h"
#include "Airplane.h"
//#include "Factory.h"

Weapon::Weapon(Airplane* owner, std::string type) {

	this->owner = owner;
	this->type = type;

	if (type.compare("Misil") == 0) {
		initMisil();
	}
}

void Weapon::initMisil() {
	this->damage = 100;
	this->bulletSpeed = 10;
	this->cooldown = 5;
	this->fireRate = 1;
	//this->mesh = Factory::buildMisil(World::instance->player->getGlobalMatrix() * Vector3(0, -1, 0));
}

void Weapon::shoot() {}

void Weapon::render() {
	/*for (int i = 0; i < this->bullets.size(); i++) {
		this->bullets[i]->render();
	}*/
	//mesh->render(World::instance->currentCamera);
}

void Weapon::update(float deltaTime) {
	this->cooldown -= deltaTime;
}