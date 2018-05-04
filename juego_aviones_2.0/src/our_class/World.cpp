#include "World.h"


World::World(const Vector3 position, Mesh* m, Texture* t, Shader* s) : EntityMesh(position, highMesh) {
	
}

World::World()
{
	this->boots = 10;
	//this->player = Factory::buildAirplane(Vector3(300, 300, 100), 20);

}

World::~World() {}

void World::render(Camera* camera) {}
/*
void World::buildWorld() {
	Mesh* worldMesh = Mesh::Load("data/island/island.ASE");
	Texture* texture = Texture::Load("data/island/island_color_luz.tga");
	Shader* shader = Shader::Load("data/shaders/texture.vs", "data/shaders/texture.fs");

	//static World* w = new World();

	//return w;
}*/






