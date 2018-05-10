#ifndef ENTIY_MESH_H
#define ENTITY_MESH_H

#include "../mesh.h"
#include "Material.h"
#include "Entity.h"

class EntityMesh : public Entity {

	public:

		Mesh* highMesh;
		Mesh* lowMesh;
		Material* material; // Texture + Shader + Vector4 color + como se pinta booleanas (blend_alpha, depth_test, cull_face)
		
		// --- CONSTRUCTORES ---

		EntityMesh(const Vector3 position);
		EntityMesh(const Vector3 position, Mesh * highMesh);
		EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh);
		EntityMesh(const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Material * material);
		EntityMesh(const Transform transform, Mesh * highMesh, Material * material);
		EntityMesh(const Vector3 position, Mesh * highMesh, Mesh * lowMesh);
		EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh);
		EntityMesh(const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Material * material);
		EntityMesh(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		virtual ~EntityMesh();

		//virtual void render();

		virtual void render(Camera * camera);

		virtual void update(float deltaTime);

		virtual void renderMesh(Camera* camera, Matrix44 globalMatrix);

	private:
		void activeGlFlags();
		void desactiveGlFlags();
};

#endif