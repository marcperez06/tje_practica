#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include "../framework.h"
#include "../extra/coldet/coldet.h"

class Entity;
class EntityMesh;
class Bullet;

class CollisionHandler {
	public:

		static bool rayCollision(Vector3 origin, Vector3 direction, EntityMesh* entityMesh, Vector3 & collision, Vector3 & normal);
		static float* getTriangleOfRayCollision(Vector3 origin, Vector3 direction, EntityMesh* mesh);
		static float* getTriangleAndPointOfRayCollision(Vector3 origin, Vector3 direction, EntityMesh* entityMesh, Vector3 & collision, Vector3 & normal);
		static bool meshCollision(EntityMesh* meshA, EntityMesh* meshB);
		static bool haveAnyRayCollision(Vector3 origin, Vector3 direction, std::vector<Entity*> entitiesMesh, Vector3 & collision, Vector3 & normal);

		static void collisionStaticEntitesAgainstDynamicEntiteis();
		static void collisionDynamicEntitesAgainstDynamicEntiteis();

		static void bulletsCollisionAgainstStaticEntities(Bullet bullets[], int bulletsSize);
		static void bulletsCollisionAgainstDynamicEntities(Bullet bullets[], int bulletsSize);
};

#endif