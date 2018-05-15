#include "CollisionHandler.h"

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