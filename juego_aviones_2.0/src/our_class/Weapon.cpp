#include "Weapon.h"
#include "Airplane.h"


Weapon::Weapon(Airplane* owner, std::string type) {

	this->owner = owner;
	this->type = type;

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