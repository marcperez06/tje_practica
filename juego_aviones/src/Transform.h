#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "framework.h"

class Transform {
	private:

		Vector3 position;
		Vector3 rotation;

	public:
		// Carpetas generales
		static const Vector3 UP;
		static const Vector3 DOWN;
		static const Vector3 LEFT;
		static const Vector3 RIGHT;

		Transform() {}
		Transform(Vector3 position, Vector3 rotation) {
			this->position = position;
			this->rotation = rotation;
		}

		Vector3 getPosition() const { return this->position; }
		Vector3 getRotation() const { return this->rotation; }

		Vector3 setPosition(Vector3 const position) { this->position = position; }
		Vector3 setRotation(Vector3 const rotation) { this->rotation = rotation; }
		
};

const  Vector3 Transform::UP = Vector3(0, 1, 0);
const  Vector3 Transform::DOWN = Vector3(0, -1, 0);
const  Vector3 Transform::LEFT = Vector3(-1, 0, 0);
const  Vector3 Transform::RIGHT = Vector3(1, 1, 0);

#endif