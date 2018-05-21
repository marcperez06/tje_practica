#include "Misil.h"
#include "Airplane.h"
#include "Factory.h"
#include "../Camera.h"
#include "World.h"
#include "CollisionHandler.h"

Misil::Misil(Airplane* owner, std::string type) : Weapon(owner, type) {
	memset(&this->misils, 0, sizeof(misils));

}

void Misil::shoot() {
	if (this->cooldown < 0) {

		Matrix44 modelMatrix = this->owner->getGlobalMatrix();
		Vector3 pos = modelMatrix * Vector3(0, -5, 0);
		Vector3 velocity = Vector3(0, 0,-150);
		velocity = velocity * this->bulletSpeed;

		Bullet misil = Factory::buildBullet(pos, velocity, 300, this->type, this->owner);

		for (int i = 0; i < maxMisil; i++) {
			Bullet& auxMisil = this->misils[i];
			if (auxMisil.timeToLive > 0) {
				continue;
			}

			auxMisil = misil;
			break;
		}

		this->cooldown = 5;
	}
}

void Misil::render() {

	Mesh mesh;
	//this->meshMisil->render();
	for (int i = 0; i < maxMisil; i++) {
		Bullet& misil = this->misils[i];
		if (misil.timeToLive > 0) {
			mesh.vertices.push_back(misil.position);
			mesh.colors.push_back(Vector4(1, 0, 0, 1));
		}
	}

	if (mesh.vertices.size() > 0) {
		glPointSize(50);
		mesh.renderFixedPipeline(GL_POINTS);
	}

}

void Misil::update(float deltaTime) {
	Weapon::update(deltaTime);

	for (int i = 0; i < maxMisil; i++) {
		Bullet& misil = this->misils[i];

		if (misil.timeToLive < 0) {
			continue;
		}

		misil.lastPosition = misil.position;
		misil.position = misil.position + misil.velocity * deltaTime;
		misil.velocity = misil.velocity + Vector3(0, -9.8, 0) * deltaTime; // aplicar gravedad
		misil.timeToLive -= deltaTime;

	}

	CollisionHandler::bulletsCollisionAgainstStaticEntities(this->misils, maxMisil);

}