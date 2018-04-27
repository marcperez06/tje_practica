#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../mesh.h"
#include "Material.h"
#include "../camera.h"
#include "Entity.h"

class GameObject : public Entity {

	public:

		Mesh* highMesh;
		Mesh* lowMesh;
		Material* material; // Texture + Shader + Vector4 color + como se pinta booleanas (blend_alpha, depth_test, cull_face)
		
		// --- CONSTRUCTORES ---

		GameObject(const Vector3 position);
		GameObject(const Vector3 position, Mesh * highMesh);
		GameObject(const Vector3 position, Mesh * highMesh, const Quaternion rotation);
		GameObject(const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Material * material);
		GameObject(const Transform transform, Mesh * highMesh, Material * material);
		GameObject(const Vector3 position, Mesh * highMesh, Mesh * lowMesh);
		GameObject(const Vector3 position, Mesh * highMesh, Mesh * lowMesh, const Quaternion rotation);
		GameObject(const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Material * material);
		GameObject(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		void render();

		void render(Camera * camera);

		virtual void uptade(float deltaTime);
};

#endif