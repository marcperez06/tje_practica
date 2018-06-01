#include "EntityCollider.h"
#include "World.h"

EntityCollider::EntityCollider(const Transform transform, Mesh * highMesh, Material * material) : EntityMesh(transform, highMesh, material) {
	this->type = ENTITY_COLLIDER;
}

EntityCollider::EntityCollider(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
								: EntityMesh(transform, highMesh, lowMesh, material) {
	this->type = ENTITY_COLLIDER;
}

void EntityCollider::update(float deltaTime) {
	this->lastPosition = this->transform.matrixModel * Vector3(0, 0, 0);
}

bool EntityCollider::testRayCollisionAgainstEntity(const Vector3 origin, const Vector3 direction, float maxRayDistance, EntityMesh* entity) {
	this->collision.origin = origin;
	this->collision.direction = direction;

	Mesh* mesh = entity->highMesh;
	if (entity->lowMesh != NULL) {
		mesh = entity->lowMesh;
	}

	this->collision.haveCollision = mesh->testRayCollision(entity->getGlobalMatrix(), origin, direction, this->collision.collisionPoint, this->collision.normalPoint, maxRayDistance);
	
	return this->collision.haveCollision;
}

bool EntityCollider::haveCollisionAgainstStaticObjects(const Vector3 origin, const Vector3 direction) {
	bool haveCollision = false;
	float maxRayDistance = direction.length();

	std::vector<Entity*> staticEntities = World::instance->staticObjects;
	for (int i = 0; (i < staticEntities.size()) && (haveCollision == false); i++) {
		haveCollision = this->testRayCollisionAgainstEntity(origin, direction, maxRayDistance, (EntityMesh*) staticEntities[i]);
	}
	return haveCollision;
}

bool EntityCollider::haveCollisionAgainstDynamicObjects(const Vector3 origin, const Vector3 direction) {
	bool haveCollision = false;
	float maxRayDistance = direction.length();
	std::vector<Entity*> dynamicEntities = World::instance->dynamicObjects;
	for (int i = 0; (i < dynamicEntities.size()) && (haveCollision == false); i++) {
		haveCollision = this->testRayCollisionAgainstEntity(origin, direction, maxRayDistance, (EntityMesh*) dynamicEntities[i]);
	}
	return haveCollision;
}

void EntityCollider::onBulletCollision(const Bullet & bullet, Vector3 collisionPoint) {}
void EntityCollider::collisionEffect() {}
