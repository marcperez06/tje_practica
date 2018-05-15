#ifndef ENTITY_COLLIDER_H
#define ENTITY_COLLIDER_H

#include "EntityMesh.h"

class EntityCollider : public EntityMesh {
	public:
		Vector3 lastPosition;

		EntityCollider(const Transform transform, Mesh * highMesh, Material * material);
		EntityCollider(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		virtual void update(float deltaTime);
};

#endif
