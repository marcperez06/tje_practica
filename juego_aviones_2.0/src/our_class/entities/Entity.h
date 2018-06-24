#ifndef ENTITY_H
#define ENTITY_H
#pragma once

#include "Transform.h"
//#include "Collision.h"
#include "../../camera.h"

enum Type {
	ENTITY,
	ENTITY_MESH,
	ENTITY_COLLIDER,
	AIRPLANE,
	WAYPOINT,
	MILITARY_BASE
};

class Entity {

	public:

		char type;

		Entity* parent;

		std::string name;
		int uuid; // universal unique identifier
		Transform transform;

		std::vector<Entity*> children;

		Entity();
		Entity(const Vector3 position);
		Entity(const Vector3 position, const Quaternion rotation);
		Entity(const Transform transform);

		~Entity();

		//virtual void render();
		virtual void render(Camera* camera);

		virtual void update(float deltaTime);

		void moveTo(const Vector3 pos);

		Vector3 getPosition();
		Vector3 getGlobalPosition();
		Matrix44 getGlobalMatrix();

		Vector3 getFront();

		void addChild(Entity* entity);
		void removeChild(Entity* entity);

		/*
		bool haveRayCollision(Vector3 origin, Vector3 direction);
		virtual void detectRayCollision(Vector3 origin, Vector3 direction);
		*/
	protected:

		virtual void renderMesh(Camera* camera, Matrix44 globalMatrix);

};

#endif