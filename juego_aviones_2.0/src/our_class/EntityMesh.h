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

		EntityMesh(const Transform transform, Mesh * highMesh, Material * material);
		EntityMesh(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		virtual ~EntityMesh();

		//virtual void render();

		virtual void render(Camera * camera);

		virtual void update(float deltaTime);

		Mesh* getCorrectMeshRespectCameraDistance(Camera* camera);

		virtual void renderMesh(Camera* camera, Matrix44 globalMatrix);

		virtual void detectRayCollision(Vector3 origin, Vector3 direction);

	private:
		void activeGlFlags();
		void desactiveGlFlags();
};

#endif