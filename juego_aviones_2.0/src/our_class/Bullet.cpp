#include "Bullet.h"
#include "../utils.h"

Bullet::Bullet() {
	this->lastPosition = Vector3(0, 0, 0);
	this->position = Vector3(0, 0, 0);
	this->velocity = Vector3(0, 0, 0);
	this->timeToLive = 0;
	this->type = "none";
}

void Bullet::render() {
	glBegin(GL_LINES);
	glColor3f(0, 1.0, 0);
	glVertex3f(this->position.x, this->position.y, this->position.z);
	glEnd();
}

void Bullet::update() {
	this->position = Vector3(this->position.x, this->position.y, this->position.z - 1);
}