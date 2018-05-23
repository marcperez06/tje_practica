#include "Bullet.h"
#include "../utils.h"

Bullet::Bullet() {
	this->lastPosition = Vector3(0, 0, 0);
	this->position = Vector3(0, 0, 0);
	this->velocity = Vector3(0, 0, 0);
	this->timeToLive = 0;
	this->type = "none";
	this->damage = 0;
}