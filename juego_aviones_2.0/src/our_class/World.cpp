#include "World.h"
#include "Factory.h"
#include <stdlib.h>

World::World()
{
	this->numEnemies = 10;
	this->initPlayer();
	this->initEnemies();
	this->initWorldMap();
}

World::~World() {
	this->enemies.clear();
	delete this->player;
	delete this->worldMap;
}

void World::initPlayer() {
	this->player = Factory::buildAirplane(Vector3(300, 300, 100), 10);
	this->player->name = "player";
	this->player->uuid = 1;
}

void World::initEnemies() {
	assert(this->player);
	if (this->player != NULL) {
		for (int i = 0; i < this->numEnemies; i++) {
			float x = (rand() % 500) + this->player->highMesh->aabb_max.x;
			float y = (rand() % 200) + 300 + this->player->highMesh->aabb_max.y;
			float z = (rand() % 50) + this->player->highMesh->aabb_max.z;
			Airplane* enemy = Factory::buildAirplane(Vector3(x, y, z), 15);
			this->enemies.push_back(enemy);
		}
	}
}

void World::initWorldMap() {
	this->worldMap = new Entity(Vector3(0, 0, 0));
	this->worldMap->name = "worldMap"; // Podem triar un nom..
	this->worldMap->uuid = 2;

	EntityMesh* island = Factory::buildIsland(Vector3(0, 0, 0));
	this->worldMap->addChild(island);

	/*
	for (int i = -2; i < 1; i++) {

		//EntityMesh* island = Factory::buildIsland(Vector3());

	}
	*/

}

void World::render(Camera* camera) {
	if (this->worldMap != NULL) {
		this->worldMap->render(camera);
	}

	if (this->player != NULL) {
		this->player->render(camera);
	}

	for (int i = 0; i < this->enemies.size(); i++) {
		if (this->enemies[i] != NULL) {
			this->enemies[i]->render(camera);
		}
	}
}

void World::update(float deltaTime) {

	if (this->worldMap != NULL) {
		this->worldMap->update(deltaTime);
	}

	if (this->player != NULL) {
		this->player->update(deltaTime);
	}

	for (int i = 0; i < this->enemies.size(); i++) {
		if (this->enemies[i] != NULL) {
			this->enemies[i]->update(deltaTime);
		}
	}

}
