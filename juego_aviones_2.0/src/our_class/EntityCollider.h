#ifndef ENTITY_COLLIDER_H
#define ENTITY_COLLIDER_H

#include "EntityMesh.h"
#include "Collision.h"

class EntityCollider : public EntityMesh {
	public:
		Vector3 lastPosition;
		Collision collision;

		EntityCollider(const Transform transform, Mesh * highMesh, Material * material);
		EntityCollider(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		virtual void update(float deltaTime);

		bool testRayCollisionAgainstEntity(const Vector3 origin, const Vector3 direction, float maxRayDistance, EntityMesh* entity);

		bool haveCollisionAgainstStaticObjects(const Vector3 origin, const Vector3 direction);
		bool haveCollisionAgainstDynamicObjects(const Vector3 origin, const Vector3 direction);

};

#endif