#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"
#include "../camera.h"

class Entity {

	public:
		Entity* parent;

		std::string name;
		int uuid; // universal unique identifier
		Transform transform;

		std::vector<Entity*> children;

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

		void addChild(Entity* entity);
		void removeChild(Entity* entity);

	protected:

		virtual void renderMesh(Camera* camera, Matrix44 globalMatrix);

};

#endif