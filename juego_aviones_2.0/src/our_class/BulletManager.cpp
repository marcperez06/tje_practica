#include "BulletManager.h"
#include "Factory.h"
#include "World.h"
#include "../extra/coldet/coldet.h"

BulletManager* BulletManager::instance = NULL;

BulletManager::BulletManager() {
	memset(&this->bullets, 0, sizeof(bullets));
	this->instance = this;
}

void BulletManager::createBullet(Vector3 pos, Vector3 velocity, std::string type, Airplane* owner) {

	Bullet bullet = *(Factory::buildBullet(pos, velocity, 10, type, owner));

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
			mesh.vertices.push_back(bullet.position);
		}
	}

	glPointSize(2);
	if (mesh.vertices.size() > 0) {
		mesh.renderFixedPipeline(GL_POINTS);
	}
}

void BulletManager::update(float deltaTime) {

	for (int i = 0; i < maxBullets; i++) {
		Bullet& bullet = this->bullets[i];
		
		if (bullet.timeToLive > 0) {
			bullet.lastPosition = bullet.position;
			bullet.position = bullet.position + bullet.velocity * deltaTime;
			bullet.velocity = bullet.velocity + Vector3(0, -4, 0) * deltaTime; // aplicar gravedad
			bullet.velocity = bullet.velocity * 0.9999; // reducir velocidad
			bullet.timeToLive -= deltaTime;
		}

	}

	this->testStaticCollisions();
	//this->testDynamicCollisions();

}

void BulletManager::testStaticCollisions() {

	std::vector<Entity*> staticEntities = World::instance->staticObjects;

	for (int i = 0; i < staticEntities.size(); i++) {

		EntityMesh* staticEntity = (EntityMesh*) staticEntities[i];
		Matrix44 modelMatrix = staticEntity->getGlobalMatrix();
		Mesh* mesh = staticEntity->highMesh;
		
		if (staticEntity->lowMesh != NULL) {
			mesh = staticEntity->lowMesh;
		}

		if (!mesh->collision_model) {
			if (!mesh->createCollisionModel()) {
				break;
			}
		}

		CollisionModel3D* collision_model = (CollisionModel3D*) mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);
		
		for (int j = 0; j < maxBullets; j++) {

			Bullet& bullet = this->bullets[i];

			if (bullet.timeToLive > 0) {

				Vector3 origin = bullet.lastPosition;
				Vector3 direction = bullet.position - bullet.lastPosition;
				float maxRayDistance = direction.length();

				if (collision_model->rayCollision(origin.v, direction.v, false, 0.0, maxRayDistance) == true) {

					bullet.timeToLive = 0;
					//staticEntity.onBulletCollision(bullet, collisionPoint); // LLamar desde testDynamicCollision, para idicarle al avion de que una bala a colisionado con el.

				}

			}

		}

	}
}