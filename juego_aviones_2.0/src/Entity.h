#ifndef ENTITY_H
#define ENTITY_H

#include "framework.h"
#include "Transform.h"

class Entity {

protected:
	std::string name;
	int uuid; // universal unique identifier
	Transform transform;

public:

	Entity(const Vector3 position) {
		this->transform = Transform(position, Quaternion());
		this->name = "";
		int uuid = -1;
	}

	Entity(const Vector3 position, const Quaternion rotation) {
		this->transform = Transform(position, rotation);
		this->name = "";
		int uuid = -1;
	}

	Entity(const Transform transform) {
		this->transform = transform;
		this->name = "";
		int uuid = -1;
	}

	Transform* getTransform() { return &this->transform; }
	std::string getName() { return this->name; }
	int getUuid() { return this->uuid; }
	
	void setTransform(Transform transform) { this->transform = transform; }
	void setName(std::string name) { this->name = name; }
	void setUuid(int uuid) { this->uuid = uuid; }

	void setPosition(Vector3 position) { this->transform.setPosition(position); }

	void move(const Vector3 pos) {
		Matrix44 newModel;
		Vector3 newPos = newModel * pos;
		this->transform.setPosition(newPos);
	}

};

#endif