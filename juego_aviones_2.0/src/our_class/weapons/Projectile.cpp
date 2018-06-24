#include "Projectile.h"
#include "../../utils.h"

Projectile::Projectile() {
	this->lastPosition = Vector3(0, 0, 0);
	this->velocity = Vector3(0, 0, 0);
	this->timeToLive = 0;
	this->type = "none";
	this->damage = 0;
}

void Projectile::setProperties(const Matrix44 transform, const Vector3 velocity, float timeToLive, std::string type, Airplane* owner, int damage) {
	this->transform = transform;
	Bullet::setProperties(this->transform.getTranslation(), velocity, timeToLive, type, owner, damage);
}