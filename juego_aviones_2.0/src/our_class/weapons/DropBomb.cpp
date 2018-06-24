#include "DropBomb.h"
#include "../entities/Airplane.h"
#include "../Factory.h"
#include "../World.h"
#include "../managers/ProjectileManager.h"
#include "../managers/SoundManager.h"

DropBomb::DropBomb(Airplane* owner, char type) : Weapon(owner, type) {

}

void DropBomb::shoot() {
	if (this->cooldown < 0 && this->ammounition > 0) {

		Matrix44 bombTransform = this->owner->getGlobalMatrix();
		bombTransform.translate(0, -2, 1);

		Vector3 velocity = Vector3(0, -1, 0);
		velocity = velocity * this->bulletSpeed;

		ProjectileManager::instance->createProjectile(bombTransform, velocity, "bomb", this->owner, this->damage);

		this->cooldown = 10;
		this->ammounition -= 1;

		SoundManager::reproduceSound("normal_shoot.wav");
	}
}