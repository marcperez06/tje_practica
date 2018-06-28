#include "CollisionHandler.h"
#include "../World.h"
#include "../entities/Airplane.h"
#include "../weapons/Projectile.h"

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

		if (staticEntity == NULL) {
			continue;
		}

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

		Vector3 boundingBoxCenter = staticEntity->getGlobalMatrix() * staticEntity->highMesh->box.center;
		Vector3 boundingBoxHalfSize = staticEntity->getGlobalMatrix() * staticEntity->highMesh->box.halfsize;

		CollisionModel3D* collision_model = (CollisionModel3D*) mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < dynamicEntities.size(); j++) {

			EntityCollider* dynamicEntity = (EntityCollider*) dynamicEntities[j];

			if (dynamicEntity == NULL || dynamicEntity->type == MILITARY_BASE) {
				continue;
			}

			Vector3 origin = dynamicEntity->lastPosition;
			Vector3 direction = dynamicEntity->getPosition() - origin;
			float maxRayDistance = direction.length();

			if (World::distanceBetween(staticEntity, dynamicEntity) > boundingBoxHalfSize.length()) {
				continue;
			}

			if (collision_model->sphereCollision(origin.v, dynamicEntity->highMesh->box.halfsize.length() - 5) == true) {
				dynamicEntity->collisionEffectAgainstStaticEntity();
			}

		}

	}

}

void CollisionHandler::collisionDynamicEntitesAgainstDynamicEntiteis() {

	std::vector<Entity*> dynamicEntities = World::instance->dynamicObjects;

	for (int i = 0; i < dynamicEntities.size(); i++) {
		boolean haveCollision = false;
		EntityCollider* dynamicEntity = (EntityCollider*)dynamicEntities[i];

		if (dynamicEntity == NULL) {
			continue;
		}

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

		Vector3 boundingBoxCenter = dynamicEntity->getGlobalMatrix() * dynamicEntity->highMesh->box.center;
		Vector3 boundingBoxHalfSize = dynamicEntity->getGlobalMatrix() * dynamicEntity->highMesh->box.halfsize;

		CollisionModel3D* collision_model = (CollisionModel3D*)mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < dynamicEntities.size(); j++) {

			EntityCollider* dynamicEntity2 = (EntityCollider*)dynamicEntities[j];

			if (dynamicEntity2 == NULL || dynamicEntity == dynamicEntity2) {
				continue;
			}

			Vector3 origin = dynamicEntity2->lastPosition;
			Vector3 direction = dynamicEntity2->getPosition() - origin;
			float maxRayDistance = direction.length();

			if (World::distanceBetween(dynamicEntity, dynamicEntity2) > boundingBoxHalfSize.length()) {
				continue;
			}

			if (collision_model->sphereCollision(origin.v, dynamicEntity2->highMesh->box.halfsize.length() - 5) == true) {
				dynamicEntity2->collisionEffectAgainstDynamicEntity();
				haveCollision = true;
			}

		}

		if (haveCollision == true) {
			dynamicEntity->collisionEffectAgainstDynamicEntity();
		}

	}

}

void CollisionHandler::bulletsCollisionAgainstStaticEntities(Bullet bullets[], int bulletsSize) {
	std::vector<Entity*> staticEntities = World::instance->staticObjects;

	for (int i = 0; i < staticEntities.size(); i++) {

		EntityMesh* staticEntity = (EntityMesh*)staticEntities[i];

		if (staticEntity == NULL) {
			continue;
		}

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

		Vector3 boundingBoxCenter = staticEntity->getGlobalMatrix() * staticEntity->highMesh->box.center;
		Vector3 boundingBoxHalfSize = staticEntity->getGlobalMatrix() * staticEntity->highMesh->box.halfsize;

		CollisionModel3D* collision_model = (CollisionModel3D*)mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < bulletsSize; j++) {

			Bullet& bullet = bullets[j];

			if (bullet.timeToLive <= 0) {
				continue;
			}

			if (bullet.position.distance(boundingBoxCenter) > boundingBoxHalfSize.length()) {
				continue;
			}

			Vector3 origin = bullet.lastPosition;
			Vector3 direction = bullet.position - bullet.lastPosition;
			float maxRayDistance = direction.length();

			if (collision_model->rayCollision(origin.v, direction.v, false, 0.0, maxRayDistance) == true) {
				bullet.timeToLive = 0;
			}

		}

	}
}

void CollisionHandler::bulletsCollisionAgainstDynamicEntities(Bullet bullets[], int bulletsSize) {
	std::vector<Entity*> dynamicEntities = World::instance->dynamicObjects;

	for (int i = 0; i < dynamicEntities.size(); i++) {

		Airplane* dynamicEntity = (Airplane*) dynamicEntities[i];

		if (dynamicEntity == NULL) {
			continue;
		}

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

		Vector3 boundingBoxCenter = dynamicEntity->getGlobalMatrix() * dynamicEntity->highMesh->box.center;
		Vector3 boundingBoxHalfSize = dynamicEntity->getGlobalMatrix() * dynamicEntity->highMesh->box.halfsize;

		CollisionModel3D* collision_model = (CollisionModel3D*)mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < bulletsSize; j++) {

			Bullet & bullet = bullets[j];

			try {
				if (bullet.timeToLive <= 0 || bullet.owner == NULL || bullet.owner == dynamicEntity
					|| bullet.owner->team == dynamicEntity->team) {
					continue;
				}

				if (bullet.position.distance(boundingBoxCenter) > boundingBoxHalfSize.length()) {
					continue;
				}
			} catch (std::exception e) { continue; }

			Vector3 origin = bullet.lastPosition;
			Vector3 direction = bullet.position - bullet.lastPosition;
			float maxRayDistance = direction.length();

			if (collision_model->sphereCollision(origin.v, dynamicEntity->highMesh->box.halfsize.length() + 5) == true) {

				Vector3 collisionPoint;
				collision_model->getCollisionPoint(collisionPoint.v, true);

				bullet.timeToLive = 0;
				dynamicEntity->onBulletCollision(bullet, collisionPoint); // LLamar desde testDynamicCollision, para idicarle al avion de que una bala a colisionado con el.

			}

		}

	}
}

void CollisionHandler::projectilesCollisionAgainstStaticEntities(Projectile projectiles[], int projectilesSize) {
	std::vector<Entity*> staticEntities = World::instance->staticObjects;

	for (int i = 0; i < staticEntities.size(); i++) {

		EntityMesh* staticEntity = (EntityMesh*)staticEntities[i];

		if (staticEntity == NULL) {
			continue;
		}

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

		Vector3 boundingBoxCenter = staticEntity->getGlobalMatrix() * staticEntity->highMesh->box.center;
		Vector3 boundingBoxHalfSize = staticEntity->getGlobalMatrix() * staticEntity->highMesh->box.halfsize;

		CollisionModel3D* collision_model = (CollisionModel3D*)mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < projectilesSize; j++) {

			Projectile & projectile = projectiles[j];

			if (projectile.timeToLive <= 0) {
				continue;
			}

			if (projectile.position.distance(boundingBoxCenter) > boundingBoxHalfSize.length()) {
				continue;
			}

			Vector3 origin = projectile.lastPosition;
			Vector3 direction = projectile.position - projectile.lastPosition;
			float maxRayDistance = direction.length();

			if (collision_model->rayCollision(origin.v, direction.v, false, 0.0, maxRayDistance) == true) {
				projectile.timeToLive = 0;
			}

		}

	}
}

void CollisionHandler::projectilesCollisionAgainstDynamicEntities(Projectile projectiles[], int projectilesSize) {
	std::vector<Entity*> dynamicEntities = World::instance->dynamicObjects;

	for (int i = 0; i < dynamicEntities.size(); i++) {

		Airplane* dynamicEntity = (Airplane*)dynamicEntities[i];

		if (dynamicEntity == NULL) {
			continue;
		}

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

		Vector3 boundingBoxCenter = dynamicEntity->getGlobalMatrix() * dynamicEntity->highMesh->box.center;
		Vector3 boundingBoxHalfSize = dynamicEntity->getGlobalMatrix() * dynamicEntity->highMesh->box.halfsize;

		CollisionModel3D* collision_model = (CollisionModel3D*)mesh->collision_model;
		assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

		collision_model->setTransform(modelMatrix.m);

		for (int j = 0; j < projectilesSize; j++) {

			Projectile & projectile = projectiles[j];

			if (projectile.timeToLive <= 0 || projectile.owner == NULL || projectile.owner == dynamicEntity
				|| projectile.owner->team == dynamicEntity->team) {
				continue;
			}

			if (projectile.position.distance(boundingBoxCenter) > boundingBoxHalfSize.length()) {
				continue;
			}

			Vector3 origin = projectile.lastPosition;
			Vector3 direction = projectile.position - projectile.lastPosition;
			float maxRayDistance = direction.length();

			if (collision_model->sphereCollision(origin.v, dynamicEntity->highMesh->box.halfsize.length() + 5) == true) {

				Vector3 collisionPoint;
				collision_model->getCollisionPoint(collisionPoint.v, true);

				projectile.timeToLive = 0;
				dynamicEntity->onBulletCollision(projectile, collisionPoint); // LLamar desde testDynamicCollision, para idicarle al avion de que una bala a colisionado con el.

			}

		}

	}
}