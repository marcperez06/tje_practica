#include "Transform.h"

// Vectores generales

const  Vector3 Transform::UP = Vector3(0, 1, 0);
const  Vector3 Transform::DOWN = Vector3(0, -1, 0);
const  Vector3 Transform::LEFT = Vector3(-1, 0, 0);
const  Vector3 Transform::RIGHT = Vector3(1, 1, 0);

Transform::Transform() {
	this->position = Vector3(0, 0, 0);
}

Transform::Transform(Vector3 const position, Quaternion rotation) {
	this->rotation = rotation;
	this->position = position;
	this->matrixModel.translate(position.x, position.y, position.z);
}

void Transform::translate(Vector3 const position) {
	this->matrixModel.translate(position.x, position.y, position.z);
	this->position = this->matrixModel.getTranslation();
}

void Transform::rotate(float angleInRadians, const Vector3 & axis) {
	this->matrixModel.rotate(angleInRadians, axis);
	this->position = this->matrixModel.getTranslation();
}