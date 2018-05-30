#include "GUI.h"
#include "World.h"
#include "Airplane.h"
#include "Weapon.h"

GUI::GUI() {}

GUI::~GUI() {}

void GUI::render() {

	this->drawPlayerWeapons();
	this->drawPlayerHealth();
	this->drawCrosshair();

}

void GUI::drawPlayerWeapons() {
	std::vector<Weapon*> weapons = World::instance->player->weapons;
	
	for (int i = 0; i < weapons.size(); i++) {

		Weapon* weapon = weapons[i];

		if (weapon->type.compare("machineGun") == 0) {
			this->drawMachineGun();
		} else if (weapon->type.compare("misil") == 0) {
			this->drawMisil();
		} else if (weapon->type.compare("bomb") == 0) {
			this->drawDropBomb();
		} else if (weapon->type.compare("shootGun") == 0) {
			this->drawShootGun();
		} else if (weapon->type.compare("machineGunFree") == 0) {
			this->drawMachineGunFree();
		}

	}
	
}

void GUI::drawMachineGun() {

}

void GUI::drawMisil() {

}

void GUI::drawDropBomb() {

}

void GUI::drawShootGun() {

}

void GUI::drawPlayerHealth() {

}

void GUI::drawCrosshair() {

}

void GUI::drawGUIElement(Texture* textue, Vector2 start, Vector2 size) {

}