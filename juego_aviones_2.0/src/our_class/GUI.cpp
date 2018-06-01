#include "GUI.h"
#include "World.h"
#include "Airplane.h"
#include "Weapon.h"

GUI::GUI(float windowWidth, float windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->shader = Shader::Load("data/shaders/gui.vs", "data/shaders/gui.fs");
}

GUI::~GUI() {}

void GUI::render() {

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->drawPlayerWeapons();
	this->drawPlayerHealth();
	this->drawCrosshair();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

}

void GUI::drawPlayerWeapons() {
	std::vector<Weapon*> weapons = World::instance->player->weapons;
	
	for (int i = 0; i < weapons.size(); i++) {

		Weapon* weapon = weapons[i];

		if (weapon->type.compare("machineGun") == 0) {
			this->drawMachineGun();
		} else if (weapon->type.compare("rocketLauncher") == 0) {
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

void GUI::drawMachineGunFree() {

}

void GUI::drawPlayerHealth() {

}

void GUI::drawCrosshair() {

	Texture* texture = Texture::Load("data/crosshair.tga");
	Vector3 center = World::instance->player->highMesh->box.center;
	Vector3 pos = World::instance->player->getGlobalMatrix() * center;
	Vector2 start = Vector2(pos.x, pos.y);
	Vector2 size = Vector2(50, 50);

	this->drawGUIElement(texture, start, size);

}

void GUI::drawGUIElement(Texture* texture, Vector2 start, Vector2 size) {

	Vector3 topLeft = Vector3(2 * start.x / this->windowWidth - 1, 2 * start.y / this->windowHeight - 1, 0);
	Vector3 topRight = Vector3(2 * (start.x + size.x) / this->windowWidth - 1, 2 * start.y / this->windowHeight - 1, 0);
	Vector3 bottomLeft = Vector3(2 * start.x / this->windowWidth - 1, 2 * (start.y + size.y) / this->windowHeight - 1, 0);
	Vector3 bottomRight = Vector3(2 * (start.x + size.x) / this->windowWidth - 1, 2 * (start.y + size.y) / this->windowHeight - 1, 0);

	Mesh element;

	element.vertices.push_back(topLeft);
	element.uvs.push_back(Vector2(0, 0));
	element.vertices.push_back(topRight);
	element.uvs.push_back(Vector2(1, 0));
	element.vertices.push_back(bottomRight);
	element.uvs.push_back(Vector2(1, 1));

	element.vertices.push_back(bottomRight);
	element.uvs.push_back(Vector2(1, 1));
	element.vertices.push_back(bottomLeft);
	element.uvs.push_back(Vector2(0, 1));
	element.vertices.push_back(topLeft);
	element.uvs.push_back(Vector2(0, 0));

	this->shader->enable();

	this->shader->setUniform4("u_color", Vector4(1, 1, 1, 1));
	this->shader->setTexture("u_texture", texture);

	element.render(GL_TRIANGLES, this->shader);

	this->shader->disable();

}

void GUI::highlightEntity(Entity* entity) {

	Vector3 pos = entity->getGlobalPosition();

	Vector3 pos2D = World::instance->currentCamera->project(pos, this->windowWidth, this->windowHeight);

	Vector2 start = Vector2(pos2D.x, pos2D.y);
	Vector2 size = Vector2(5, 5);

	Texture* texture = Texture::Load("data/texture.tga");
	this->drawGUIElement(texture, start, size);

}