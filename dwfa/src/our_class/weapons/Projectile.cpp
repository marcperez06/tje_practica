#include "Projectile.h"
#include "../../utils.h"

Projectile::Projectile() : Bullet() {
	this->transform = Matrix44();
}

void Projectile::setProperties(const Matrix44 transform, const Vector3 velocity, float timeToLive, std::string type, Airplane* owner, int damage) {
	this->transform = transform;
	Bullet::setProperties(this->transform.getTranslation(), velocity, timeToLive, type, owner, damage);
}