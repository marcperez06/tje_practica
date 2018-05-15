#include "EntityCollider.h"

EntityCollider::EntityCollider(const Transform transform, Mesh * highMesh, Material * material) : EntityMesh(transform, highMesh, material) {

}

EntityCollider::EntityCollider(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
								: EntityMesh(transform, highMesh, lowMesh, material) {
}

void EntityCollider::update(float deltaTime) {
	this->lastPosition = this->transform.matrixModel * Vector3(0, 0, 0);
}