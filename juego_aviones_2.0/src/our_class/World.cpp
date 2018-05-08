#include "World.h"
#include "Factory.h"
#include <stdlib.h>

World::World()
{
	this->numEnemies = 100;
	this->initPlayer();
	this->initEnemies();
	this->initWorldMap();
	this->initSky();
}

World::~World() {
	this->enemies.clear();
	delete this->player;
	delete this->worldMap;
	delete this->sky;
}

void World::initPlayer() {
	this->player = Factory::buildAirplane(Vector3(300, 300, 100), 30);
	this->player->name = "player";
	this->player->uuid = 1;
}

void World::initEnemies() {
	assert(this->player);
	if (this->player != NULL) {
		for (int i = 0; i < this->numEnemies; i++) {
			float x = (rand() % 300) + this->player->highMesh->aabb_max.x;
			float y = (rand() % 200) + 300 + this->player->highMesh->aabb_max.y;
			float z = (rand() % 20) + this->player->highMesh->aabb_max.z;
			Airplane* enemy = Factory::buildAirplane(Vector3(x, y, z), 15);
			this->enemies.push_back(enemy);
		}
	}
}

void World::initWorldMap() {
	this->worldMap = new Entity(Vector3(0, 0, 0));
	this->worldMap->name = "worldMap"; // Podem triar un nom..
	this->worldMap->uuid = 2;

	Mesh * meshIsland = Mesh::Load("data/island/island.ASE");
	float maxX = (meshIsland->aabb_max.x * 2) - 10;
	float maxZ = (meshIsland->aabb_max.z * 2) - 10;
	//delete meshIsland;

	//this->worldMap->addChild(island);

	for (int i = -2; i < 1; i++) {

		for (int j = -2; j < 2; j++) {
			float x = maxX * i;
			float y = 0;
			float z = maxZ * j;

			EntityMesh* island = Factory::buildIsland(Vector3(x, y, z));
			this->worldMap->addChild(island);

		}
	}
}

void World::initSky() {
	assert(this->player);
	if (this->player != NULL) {
		this->sky = Factory::buildSky(this->player->getGlobalPosition());
	}
}

void World::renderWorldMap(Camera* camera) {
	EntityMesh* base = NULL;
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/fog.fs");
	
	Mesh * water = Mesh::Load("data/island/water_deep.ASE");

	shader->enable();

	if (this->worldMap->children.size() > 0) {
		base = (EntityMesh*) this->worldMap->children[0];
	}

	if (base != NULL) {
		shader->setUniform("u_color", base->material->color);
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", base->material->texture);
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", 1);
		
		base->highMesh->enableBuffers(shader);

		for (int i = 0; i < this->worldMap->children.size(); i++) {
			shader->setUniform("u_model", this->worldMap->children[i]->getGlobalMatrix());
			base->highMesh->drawCall(GL_TRIANGLES);
			water->drawCall(GL_TRIANGLES);
		}
		
		base->highMesh->disableBuffers(shader);

	}

	shader->disable();

}

void World::renderEnemies(Camera* camera) {
	Airplane* base;
	std::vector<Matrix44> enemiesPos;
	Mesh* enemyMesh = Mesh::Load("data/spitfire/spitfire.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");

	if (this->enemies.size() > 0) {
		base = (Airplane*) this->enemies[0];
	}

	for (int i = 0; i < this->enemies.size(); i++) { enemiesPos.push_back(this->enemies[i]->getGlobalMatrix()); }

	shader->enable();

	if (enemyMesh != NULL) {
		shader->setUniform("u_color", base->material->color);
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", base->material->texture);
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", 1);

		enemyMesh->renderInstanced(GL_TRIANGLES, shader, &enemiesPos[0], enemiesPos.size());

	}

	shader->disable();

}

void World::render(Camera* camera) {
	if (this->sky != NULL) {
		this->sky->render(camera);
	}

	if (this->worldMap != NULL) {
		//this->worldMap->render(camera);
		this->renderWorldMap(camera);
	}

	if (this->player != NULL) {
		this->player->render(camera);
	}

	this->renderEnemies(camera);
	/*for (int i = 0; i < this->enemies.size(); i++) {
		if (this->enemies[i] != NULL) {
			this->enemies[i]->render(camera);
		}
	}*/
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

	if (this->sky != NULL) {
		this->sky->update(deltaTime);
	}

}
