#include "GUI.h"
#include "World.h"
#include "Airplane.h"
#include "Weapon.h"
#include "../utils.h"

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
	this->drawPlayerFuell();
	this->drawCrosshair();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

}

void GUI::drawPlayerWeapons() {
	std::vector<Weapon*> weapons = World::instance->player->weapons;
	
	for (int i = 0; i < weapons.size(); i++) {

		Weapon* weapon = weapons[i];

		if (weapon->type == MACHINE_GUN) {
			this->drawMachineGun();
		} else if (weapon->type == ROCKET_LAUNCHER) {
			this->drawMisil();
		} else if (weapon->type == DROP_BOMB) {
			this->drawDropBomb();
		} else if (weapon->type == SHOOT_GUN) {
			this->drawShootGun();
		} else if (weapon->type == MACHINE_GUN) {
			this->drawMachineGunFree();
		}

	}
	
}

void GUI::drawMachineGun() {
	Texture* texture = Texture::Load("data/texture.tga");
	Vector2 start = Vector2(5, 30);
	Vector2 size = Vector2(50, 50);
	
	this->drawGUIElement(texture, start, size);
}

void GUI::drawMisil() {
	Texture* texture = Texture::Load("data/texture.tga");
	Vector2 start = Vector2(5, 80);
	Vector2 size = Vector2(50, 50);

	this->drawGUIElement(texture, start, size);
}

void GUI::drawDropBomb() {
	Texture* texture = Texture::Load("data/texture.tga");
	Vector2 start = Vector2(5, 130);
	Vector2 size = Vector2(50, 50);

	this->drawGUIElement(texture, start, size);
}

void GUI::drawShootGun() {
	Texture* texture = Texture::Load("data/texture.tga");
	Vector2 start = Vector2(5, 180);
	Vector2 size = Vector2(50, 50);

	this->drawGUIElement(texture, start, size);
}

void GUI::drawMachineGunFree() {
	Texture* texture = Texture::Load("data/texture.tga");
	Vector2 start = Vector2(5, 180);
	Vector2 size = Vector2(50, 50);

	this->drawGUIElement(texture, start, size);
}

void GUI::drawPlayerHealth() {
	std::ostringstream floatToStr;
	floatToStr << "Health: " << World::instance->player->health;
	
	std::string playerHealth(floatToStr.str());

	drawText(5, this->windowHeight - 20, playerHealth, Vector3(1, 1, 1), 2);
}

void GUI::drawPlayerFuell() {
	std::ostringstream floatToStr;
	floatToStr << "Fuell: " << World::instance->player->fuell;

	std::string playerHealth(floatToStr.str());

	drawText(this->windowWidth - 175, this->windowHeight - 20, playerHealth, Vector3(1, 1, 1), 2);
}

void GUI::drawCrosshair() {

	Texture* texture = Texture::Load("data/crosshair.tga");
	Vector3 center = World::instance->player->highMesh->box.center + Vector3(-0.58, 1.76, -3);
	Vector3 pos = World::instance->player->getGlobalMatrix() * center;
	//Vector3 pos = World::instance->player->getGlobalPosition() + Vector3(-1.05, 0.75, -3);
	Vector3 pos2D = World::instance->playerCamera->project(pos, this->windowWidth, this->windowHeight);
	Vector2 start = Vector2(pos2D.x, pos2D.y);
	Vector2 size = Vector2(50, 50);

	if (pos2D.z < 1) {
		this->drawGUIElement(texture, start, size);
	}

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
	Vector2 size = Vector2(30, 30);

	Texture* texture = Texture::Load("data/texture.tga");
	
	if (pos2D.z < 1) {
		this->drawGUIElement(texture, start, size);
	}

}