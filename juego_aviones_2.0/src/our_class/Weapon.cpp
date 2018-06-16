#include "Weapon.h"
#include "Airplane.h"


Weapon::Weapon(Airplane* owner, char type) {
	this->owner = owner;
	this->type = type;
	this->ammounition = 0;
}

void Weapon::shoot() {}

void Weapon::update(float deltaTime) {
	this->cooldown -= deltaTime;
}

void Weapon::addAmmounition(int ammo) {
	this->ammounition += ammo;
}