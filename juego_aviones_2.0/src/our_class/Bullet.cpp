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

void Bullet::setProperties(const Vector3 pos, const Vector3 velocity, float timeToLive, std::string type, Airplane* owner, int damage) {
	this->lastPosition = pos;
	this->position = pos;
	this->velocity = velocity;
	this->timeToLive = timeToLive;
	this->type = type;
	this->owner = owner;
	this->damage = damage;
}