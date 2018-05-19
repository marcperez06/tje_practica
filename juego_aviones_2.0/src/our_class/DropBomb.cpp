#include "DropBomb.h"
#include "Airplane.h"
#include "Factory.h"
#include "World.h"
#include "../extra/coldet/coldet.h"

DropBomb::DropBomb(Airplane* owner, std::string type) : Weapon(owner, type) {}

void DropBomb::shoot() {
	if (this->cooldown < 0) {

		Matrix44 modelMatrix = this->owner->getGlobalMatrix();
		Vector3 pos = modelMatrix * Vector3(0, -2, 1);
		Vector3 velocity = modelMatrix.rotateVector(Vector3(0, -1, 0));
		velocity = velocity * this->owner->speed * this->bulletSpeed;

		Bullet bomb = Factory::buildBullet(pos, velocity, 50, this->type, this->owner);
		this->bullets.push_back(bomb);

		this->cooldown = 10;
	}
}

void DropBomb::update(float deltaTime) {
	Weapon::update(deltaTime);

	for (int i = 0; i < this->bullets.size(); i++) {
		Bullet& bullet = this->bullets[i];

		if (bullet.timeToLive < 0) {
			continue;
		}

		bullet.lastPosition = bullet.position;
		bullet.position = bullet.position + bullet.velocity * deltaTime;
		bullet.velocity = bullet.velocity + Vector3(0, -9.8, 0) * deltaTime; // aplicar gravedad
		bullet.velocity = bullet.velocity * 1; // reducir velocidad
		bullet.timeToLive -= deltaTime;

	}

	this->testStaticCollisions();

}

void DropBomb::render() {

	Mesh mesh;

	for (int i = 0; i < bullets.size(); i++) {
		Bullet& bullet = this->bullets[i];
		if (bullet.timeToLive > 0) {
			mesh.vertices.push_back(bullet.position);
			mesh.colors.push_back(Vector4(0, 1, 0, 1));
		}
	}

	if (mesh.vertices.size() > 0) {
		glPointSize(30);
		mesh.renderFixedPipeline(GL_POINTS);
	}
	
}

void DropBomb::testStaticCollisions() {

	std::vector<Entity*> staticEntities = World::instance->staticObjects;

	for (int i = 0; i < staticEntities.size(); i++) {

		EntityMesh* staticEntity = (EntityMesh*)staticEntities[i];
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

		for (int j = 0; j < bullets.size(); j++) {

			Bullet& bullet = this->bullets[j];

			if (bullet.timeToLive < 0) {
				continue;
			}

			Vector3 origin = bullet.lastPosition;
			Vector3 direction = bullet.position - bullet.lastPosition;
			float maxRayDistance = direction.length();

			if (collision_model->rayCollision(origin.v, direction.v, false, 0.0, maxRayDistance) == true) {

				bullet.timeToLive = 0;
				std::cout << "BUllet Destroy ..... " << std::endl;
				//staticEntity.onBulletCollision(bullet, collisionPoint); // LLamar desde testDynamicCollision, para idicarle al avion de que una bala a colisionado con el.

			}

		}

	}
}