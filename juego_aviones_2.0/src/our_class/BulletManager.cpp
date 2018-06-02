#include "BulletManager.h"
#include "Factory.h"
#include "World.h"
#include "../extra/coldet/coldet.h"
#include "EntityCollider.h"
#include "CollisionHandler.h"

BulletManager* BulletManager::instance = NULL;

BulletManager::BulletManager() {
	memset(&this->bullets, 0, sizeof(bullets));
	this->instance = this;
}

void BulletManager::createBullet(Vector3 pos, Vector3 velocity, std::string type, Airplane* owner, int damage) {

	Bullet bullet;
	bullet.setProperties(pos, velocity, 10, type, owner, damage);

	for (int i = 0; i < maxBullets; i++) {
		Bullet& auxBullet = this->bullets[i];
		if (auxBullet.timeToLive > 0) {
			continue;
		}

		auxBullet = bullet;
		break;
	}

}

void BulletManager::render() {

	Mesh mesh;

	for (int i = 0; i < maxBullets; i++) {
		Bullet& bullet = this->bullets[i];
		if (bullet.timeToLive > 0) {
			mesh.vertices.push_back(bullet.lastPosition);
			mesh.vertices.push_back(bullet.position);
		}
	}

	if (mesh.vertices.size() > 0) {
		mesh.renderFixedPipeline(GL_LINES);
	}
	 
}

void BulletManager::update(float deltaTime) {

	for (int i = 0; i < maxBullets; i++) {
		Bullet& bullet = this->bullets[i];
		
		if (bullet.timeToLive < 0) {
			continue;
		}

		bullet.lastPosition = bullet.position;
		bullet.position = bullet.position + bullet.velocity * deltaTime;
		bullet.velocity = bullet.velocity + Vector3(0, -4, 0) * deltaTime; // aplicar gravedad
		bullet.velocity = bullet.velocity * 0.9999; // reducir velocidad
		bullet.timeToLive -= deltaTime;

	}

	CollisionHandler::bulletsCollisionAgainstStaticEntities(this->bullets, maxBullets);
	CollisionHandler::bulletsCollisionAgainstDynamicEntities(this->bullets, maxBullets);
	//this->testDynamicCollisions();

}