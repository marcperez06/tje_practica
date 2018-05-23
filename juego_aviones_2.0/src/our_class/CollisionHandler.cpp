#include "CollisionHandler.h"
#include "World.h"
#include "Airplane.h"
#include "Bullet.h"

bool CollisionHandler::rayCollision(Vector3 origin, Vector3 direction, EntityMesh* entityMesh, Vector3 & collision, Vector3 & normal) {
	bool haveCollision = false;
	
	if (entityMesh != NULL) {

		Mesh* mesh = entityMesh->highMesh;
		if (entityMesh->lowMesh != NULL) {
			mesh = entityMesh->lowMesh;
		}

		if (mesh != NULL) {
			haveCollision = mesh->testRayCollision(entityMesh->getGlobalMatrix(), origin, direction, collision, normal);
		}
	}
	return haveCollision;
}

bool CollisionHandler::haveAnyRayCollision(Vector3 origin, Vector3 direction, std::vector<Entity*> entitiesMesh, Vector3 & collision, Vector3 & normal) {
	bool haveCollision = false;
	for (int i = 0; (i < entitiesMesh.size()) && (haveCollision == false); i++) {
		if (entitiesMesh[i] != NULL) {
			haveCollision = rayCollision(origin, direction, (EntityMesh*) entitiesMesh[i], collision, normal);
		}
	}
	return haveCollision;
}

float* CollisionHandler::getTriangleOfRayCollision(Vector3 origin, Vector3 direction, EntityMesh* entityMesh) {
	float triangle[9];
	for (int i = 0; i < 9; i++) { triangle[i] = -1; }
	if ((entityMesh != NULL) && (entityMesh->highMesh != NULL)) {
		if (entityMesh->highMesh->collision_model == NULL) { entityMesh->highMesh->createCollisionModel(); }
		CollisionModel3D* collision_model = (CollisionModel3D*) entityMesh->highMesh->collision_model;
		collision_model->setTransform(entityMesh->getGlobalMatrix().m);
		
		if (collision_model->rayCollision(origin.v, direction.v, true) == true) {
			float t2[9];
			// Almacenar triangulo de la colision.
			collision_model->getCollidingTriangles(triangle, t2, false);
		}
	}
	return triangle;
}

float* CollisionHandler::getTriangleAndPointOfRayCollision(Vector3 origin, Vector3 direction, EntityMesh* mesh, Vector3 & collision, Vector3 & normal) {
	float* triangle;
	
	if ((mesh != NULL) && (mesh->highMesh != NULL)) {
		CollisionHandler::rayCollision(origin, direction, mesh, collision, normal);
		triangle = CollisionHandler::getTriangleOfRayCollision(origin, direction, mesh);
	}

	return triangle;
}

bool CollisionHandler::meshCollision(EntityMesh* meshA, EntityMesh* meshB) {
	bool haveCollision = false;
	CollisionModel3D* collisionModelA = NULL;
	CollisionModel3D* collisionModelB = NULL;
	if ((meshA != NULL) && (meshB != NULL) && (meshA->highMesh != NULL) && (meshB->highMesh != NULL)) {
		if (meshA->highMesh->collision_model == NULL) { meshA->highMesh->createCollisionModel(); }
		if (meshB->highMesh->collision_model == NULL) { meshB->highMesh->createCollisionModel(); }
		collisionModelA = (CollisionModel3D*) meshA->highMesh->collision_model;
		collisionModelB = (CollisionModel3D*) meshB->highMesh->collision_model;
		collisionModelA->setTransform(meshA->getGlobalMatrix().m);
		haveCollision = collisionModelA->collision(collisionModelB, -1, 0, meshB->getGlobalMatrix().m);
	}
	return haveCollision;
}

void CollisionHandler::collisionStaticEntitesAgainstDynamicEntiteis() {

	std::vector<Entity*> staticEntities = World::instance->staticObjects;
	std::vector<Entity*> dynamicEntities = World::instance->dynamicObjects;

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

		for (int j = 0; j < dynamicEntities.size(); j++) {

			EntityCollider* dynamicEntity = (EntityCollider*) dynamicEntities[j];

			Vector3 origin = dynamicEntity->lastPosition;
			Vector3 direction = dynamicEntity->getPosition() - origin;
			float maxRayDistance = direction.length();

			if (collision_model->rayCollision(origin.v, direction.v, false, 0.0, maxRayDistance) == true) {

				std::cout << "Airplane Destroy ..... " << std::endl;
				dynamicEntity->collisionEffect();

			}

		}

	}

}

void CollisionHandler::bulletsCollisionAgainstStaticEntities(Bullet bullets[], int bulletsSize) {
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

		CollisionModel3D* collision_model = (CollisionModel3D*)mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < bulletsSize; j++) {

			Bullet& bullet = bullets[j];

			if (bullet.timeToLive <= 0) {
				continue;
			}

			Vector3 origin = bullet.lastPosition;
			Vector3 direction = bullet.position - bullet.lastPosition;
			float maxRayDistance = direction.length();

			if (collision_model->rayCollision(origin.v, direction.v, false, 0.0, maxRayDistance) == true) {

				bullet.timeToLive = 0;
				std::cout << "BUllet Destroy ..... " << std::endl;

			}

		}

	}
}

void CollisionHandler::bulletsCollisionAgainstDynamicEntities(Bullet bullets[], int bulletsSize) {
	std::vector<Entity*> dynamicEntities = World::instance->dynamicObjects;

	for (int i = 0; i < dynamicEntities.size(); i++) {

		Airplane* dynamicEntity = (Airplane*) dynamicEntities[i];
		Matrix44 modelMatrix = dynamicEntity->getGlobalMatrix();
		Mesh* mesh = dynamicEntity->highMesh;

		if (dynamicEntity->lowMesh != NULL) {
			mesh = dynamicEntity->lowMesh;
		}

		if (!mesh->collision_model) {
			if (!mesh->createCollisionModel()) {
				break;
			}
		}

		CollisionModel3D* collision_model = (CollisionModel3D*)mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < bulletsSize; j++) {

			Bullet& bullet = bullets[j];

			if (bullet.timeToLive <= 0) {
				continue;
			}

			Vector3 origin = bullet.lastPosition;
			Vector3 direction = bullet.position - bullet.lastPosition;
			float maxRayDistance = direction.length();

			if (collision_model->rayCollision(origin.v, direction.v, false, 0.0, maxRayDistance) == true) {

				Vector3 collisionPoint;
				collision_model->getCollisionPoint(collisionPoint.v, true);

				bullet.timeToLive = 0;
				std::cout << "BUllet Destroy ..... " << std::endl;
				dynamicEntity->onBulletCollision(bullet, collisionPoint); // LLamar desde testDynamicCollision, para idicarle al avion de que una bala a colisionado con el.

			}

		}

	}
}