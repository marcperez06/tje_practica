#include "DropBomb.h"
#include "Airplane.h"
#include "Factory.h"
#include "World.h"
#include "CollisionHandler.h"

DropBomb::DropBomb(Airplane* owner, std::string type) : Weapon(owner, type) {
	memset(&this->bombs, 0, sizeof(bombs));
}

void DropBomb::shoot() {
	if (this->cooldown < 0) {

		Matrix44 modelMatrix = this->owner->getGlobalMatrix();
		Vector3 pos = modelMatrix * Vector3(0, -2, 1);
		Vector3 velocity = Vector3(0, -1, 0);
		velocity = velocity * this->bulletSpeed;

		Bullet bomb = Factory::buildBullet(pos, velocity, 300, this->type, this->owner, this->damage);

		for (int i = 0; i < maxBombs; i++) {
			Bullet& auxBomb = this->bombs[i];
			if (auxBomb.timeToLive > 0) {
				continue;
			}

			auxBomb = bomb;
			break;
		}

		this->cooldown = 10;
	}
}

void DropBomb::render() {

	Mesh mesh;

	for (int i = 0; i < maxBombs; i++) {
		Bullet& bomb = this->bombs[i];
		if (bomb.timeToLive > 0) {
			mesh.vertices.push_back(bomb.position);
			mesh.colors.push_back(Vector4(1, 0, 0, 1));
		}
	}

	if (mesh.vertices.size() > 0) {
		glPointSize(30);
		mesh.renderFixedPipeline(GL_POINTS);
	}

}

void DropBomb::update(float deltaTime) {
	Weapon::update(deltaTime);

	for (int i = 0; i < maxBombs; i++) {
		Bullet& bomb = this->bombs[i];

		if (bomb.timeToLive < 0) {
			continue;
		}

		bomb.lastPosition = bomb.position;
		bomb.position = bomb.position + bomb.velocity * deltaTime;
		bomb.velocity = bomb.velocity + Vector3(0, -9.8, 0) * deltaTime; // aplicar gravedad
		bomb.timeToLive -= deltaTime;

	}

	CollisionHandler::bulletsCollisionAgainstStaticEntities(this->bombs, maxBombs);

}