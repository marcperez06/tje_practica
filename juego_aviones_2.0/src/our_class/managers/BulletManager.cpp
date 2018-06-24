#include "BulletManager.h"
#include "../World.h"
#include "../../extra/coldet/coldet.h"
#include "../entities/EntityCollider.h"
#include "CollisionHandler.h"

BulletManager* BulletManager::instance = NULL;

BulletManager::BulletManager() {
	memset(&this->bullets, 0, sizeof(bullets));
	this->instance = this;
}

void BulletManager::createBullet(Vector3 pos, Vector3 velocity, std::string type, Airplane* owner, int damage) {

	Bullet bullet;
	int timeToLive = 0;

	if (type.compare("machineGun") == 0) {
		timeToLive = 10;
	}	else if (type.compare("shootGun") == 0) {
		timeToLive = 1;
	}

	bullet.setProperties(pos, velocity, timeToLive, type, owner, damage);

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
			mesh.colors.push_back(Vector4(255.0 / 255.0, 249.0 / 255.0, 147.0 / 255.0, 1));
			mesh.colors.push_back(Vector4(255.0 / 255.0, 249.0 / 255.0, 147.0 / 255.0, 1));
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