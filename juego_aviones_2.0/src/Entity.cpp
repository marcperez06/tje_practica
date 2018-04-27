#include "our_class/Entity.h"

Entity::Entity(const Vector3 position) {
	this->transform = Transform(position, Quaternion());
	this->name = "";
	this->uuid = -1;
}

Entity::Entity(const Vector3 position, const Quaternion rotation) {
	this->transform = Transform(position, rotation);
	this->name = "";
	this->uuid = -1;
}

Entity::Entity(const Transform transform) {
	this->transform = transform;
	this->name = "";
	this->uuid = -1;
}

void Entity::moveTo(const Vector3 pos) {
	Matrix44 newModel;
	Vector3 newPos = newModel * pos;
	this->transform.translate(newPos);
}