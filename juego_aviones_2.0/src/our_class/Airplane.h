#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "GameObject.h"

class Airplane : public GameObject {

	public:

		float speed;
		float health;
	
		// std::vector<Wepon*> wepons;
		// int currentWepon;

		// --- CONSTRUCTORES ---

		Airplane(const float speed, const Vector3 position);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, const Quaternion rotation);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Material * material);
		Airplane(const float speed, const Transform transform, Mesh * highMesh, Material * material);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh, const Quaternion rotation);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Material * material);
		Airplane(const float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		virtual void uptade(float deltaTime);
};

#endif