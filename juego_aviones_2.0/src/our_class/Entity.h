#ifndef ENTITY_H
#define ENTITY_H

#include "../framework.h"
#include "Transform.h"

class Entity {

	public:

		std::string name;
		int uuid; // universal unique identifier
		Transform transform;

		Entity(const Vector3 position);
		Entity(const Vector3 position, const Quaternion rotation);
		Entity(const Transform transform);

		void moveTo(const Vector3 pos);

};

#endif