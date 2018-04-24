#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "framework.h"
#include "Transform.h"
#include "mesh.h"

class GameObject {

	protected:
		Transform transform;
		Mesh* mesh;
		//Material* material; // Texture + Shader + Vector4 color + como se pinta booleanas (blend_alpha, depth_test, cull_face)
		std::string name;
		int uuid; // universal unique identifier

	public:
		GameObject(const Vector3 position, Mesh * mesh) {
			this->transform = Transform(position, Quaternion());
			this->mesh = mesh;
		}

		Transform* getTransform() { std::cout << "Get Transform" << std::endl; return &this->transform; }
		Mesh* getMesh() { return this->mesh; }
	
		void setPosition(Vector3 position) {
			this->transform.setLocalPosition(position);
		}
	
		void setMesh(Mesh * mesh) { this->mesh = mesh; }

		void move(const Vector3 pos) {
			Matrix44 newModel;
			Vector3 newPos = newModel * pos;
			this->transform.setLocalPosition(newPos);
		}

};

#endif