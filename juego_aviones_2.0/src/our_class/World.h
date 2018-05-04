#ifndef WORLD_H
#define WORLD_H

#include "../framework.h"
#include "../utils.h"
#include "../mesh.h"
#include "../texture.h"
#include "../fbo.h"
#include "../shader.h"
#include "../input.h"

#include "Airplane.h"
#include "Factory.h"
#include "EntityMesh.h"
#include "Camera.h"
class World
{
	public:

		//Airplane* player;
		int boots;

		World();
		World::World(const Vector3 position, Mesh* m, Texture* t, Shader* s);
		virtual ~World();

		virtual void render(Camera* camera);
		//void World::buildWorld();

};


#endif