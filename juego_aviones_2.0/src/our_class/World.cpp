#include "World.h"
#include "Factory.h"
#include <stdlib.h>
#include "../game.h";
#include "BulletManager.h"

World* World::instance = NULL;

World::World() {
	this->root = new Entity(Vector3(0, 0, 0));
	this->numEnemies = 100;
	this->initPlayer();
	this->initCameras();
	this->initEnemies();
	this->initWorldMap();
	this->initSky();
	World::instance = this;
}

World::~World() {
	for (int i = 0; i < this->root->children.size(); i++) {
		delete this->root->children[i];
		this->root->children.erase(this->root->children.begin() + i);
	}
	delete this->player;
	delete this->sky;
}

void World::initCameras() {
	Vector3 cameraPosition = this->player->transform.matrixModel * Vector3(0, 3, 9);
	Vector3 cameraCenter = this->player->transform.matrixModel * (this->player->highMesh->box.center + Vector3(0, 0, 1));
	Vector3 cameraUp = this->player->transform.matrixModel.rotateVector(Transform::UP);

	//create our free camera
	this->freeCamera = new Camera();
	this->freeCamera->lookAt(cameraPosition, cameraCenter, cameraUp); //position the camera and point to 0,0,0
	this->freeCamera->setPerspective(70.f, Game::instance->window_width / (float) Game::instance->window_height, 0.1f, 30000.f); //set the projection, we want to be perspective

	this->playerCamera = new Camera();
	this->playerCamera->lookAt(cameraPosition, cameraCenter, cameraUp); //position the camera and point to 0,0,0
	this->playerCamera->setPerspective(70.f, Game::instance->window_width / (float) Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	this->currentCamera = this->playerCamera;
}

void World::initPlayer() {
	this->player = Factory::buildAirplane(Vector3(0, 500, 0), 200);
	this->player->name = "player";
	this->player->uuid = 1;
	this->player->weapons.push_back(new Weapon(this->player->uuid, "Misil"));
	this->player->currentWepon = 0;
}

void World::initEnemies() {
	assert(this->player);
	if (this->player != NULL) {
		for (int i = 0; i < this->numEnemies; i++) {
			float x = (rand() % 300) + this->player->highMesh->aabb_max.x;
			float y = (rand() % 200) + 300 + this->player->highMesh->aabb_max.y;
			float z = (rand() % 20) + this->player->highMesh->aabb_max.z;
			Airplane* enemy = Factory::buildAirplane(Vector3(x, y, z), 15);
			this->root->addChild(enemy);
			this->dinamicObjects.push_back(enemy);
		}
	}
}

void World::initWorldMap() {
	Entity* worldMap = new Entity(Vector3(0, 0, 0));
	worldMap->name = "worldMap"; // Podem triar un nom..
	worldMap->uuid = 2;

	Mesh * meshIsland = Mesh::Load("data/island/island.ASE");
	float maxX = (meshIsland->aabb_max.x * 2) - 10;
	float maxZ = (meshIsland->aabb_max.z * 2) - 10;
	//delete meshIsland;

	for (int i = -2; i < 1; i++) {

		for (int j = -2; j < 2; j++) {
			float x = maxX * i;
			float y = 0;
			float z = maxZ * j;

			EntityMesh* island = Factory::buildIsland(Vector3(x, y, z));
			worldMap->addChild(island);
			this->staticObjects.push_back(island);
		}
	}

	this->root->addChild(worldMap);
}

void World::initSky() {
	assert(this->playerCamera);
	this->sky = Factory::buildSky(this->playerCamera->eye);
}

/*

void World::renderWorldMap(Camera* camera) {
	EntityMesh* base = NULL;
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/world.fs");
	
	Mesh * water = Mesh::Load("data/island/water_deep.ASE");

	shader->enable();

	Entity* worldMap = this->getWorldMap();

	if (worldMap->children.size() > 0) {
		base = (EntityMesh*) worldMap->children[0];
	}

	if (base != NULL) {
		shader->setUniform("u_color", base->material->color);
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", base->material->texture);
		shader->setUniform("u_detail_texture", Texture::Load("data/island/rock.tga"));
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", 1);
		
		base->highMesh->enableBuffers(shader);

		for (int i = 0; i < worldMap->children.size(); i++) {
			shader->setUniform("u_model", worldMap->children[i]->getGlobalMatrix());
			base->highMesh->drawCall(GL_TRIANGLES);
			water->drawCall(GL_TRIANGLES);
		}
		
		base->highMesh->disableBuffers(shader);

	}

	shader->disable();

}

*/

/* No acabar de funcionar correctament pintar el mapa utilitzant instancing per reduir el nombre de drawCalls*/
void World::renderWorldMap(Camera* camera) {
	EntityMesh* base;
	std::vector<Matrix44> islandsPos;
	Mesh* islandMesh = Mesh::Load("data/island/island.ASE");
	Mesh* waterMesh = Mesh::Load("data/island/water_deep.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/world.fs");

	if (this->getWorldMap()->children.size() > 0) {
		base = (EntityMesh*) this->getWorldMap()->children[0];
		//islandMesh = base->getCorrectMeshRespectCameraDistance(camera);
	}

	for (int i = 0; i < this->getWorldMap()->children.size(); i++) {
		EntityMesh* island = (EntityMesh*) this->getWorldMap()->children[i];
		islandsPos.push_back(island->getGlobalMatrix());
	}

	if (shader != NULL) {
		shader->enable();

		if (islandMesh != NULL) {
			shader->setUniform("u_color", base->material->color);
			shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
			shader->setUniform("u_texture", base->material->texture);
			shader->setUniform("u_detail_texture", Texture::Load("data/island/rock.tga"));
			shader->setUniform("u_camera_position", camera->eye);
			shader->setUniform("u_time", 1);

			islandMesh->renderInstanced(GL_TRIANGLES, shader, &islandsPos[0], islandsPos.size());

			shader->setUniform("u_texture", Texture::Load("data/island/water_deep.tga"));

			//waterMesh->renderInstanced(GL_TRIANGLES, shader, &islandsPos[0], islandsPos.size());

		}

		shader->disable();
	}

	//renderWater(camera);

}

void World::renderWater(Camera* camera) {
	EntityMesh* base;
	std::vector<Matrix44> waterPos;
	Mesh* waterMesh = Mesh::Load("data/island/water_deep.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");
	Texture* waterTexture = Texture::Load("data/island/water_deep.tga");

	if (this->getWorldMap()->children.size() > 0) {
		base = (EntityMesh*) this->getWorldMap()->children[0];
	}

	for (int i = 0; i < this->getWorldMap()->children.size(); i++) {
		EntityMesh* island = (EntityMesh*) this->getWorldMap()->children[i];
		waterPos.push_back(island->getGlobalMatrix());
	}

	if (shader != NULL) {
		shader->enable();

		if (waterMesh != NULL) {
			shader->setUniform("u_color", base->material->color);
			shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
			shader->setUniform("u_camera_position", camera->eye);
			shader->setUniform("u_time", 1);
			shader->setUniform("u_texture", waterTexture);

			waterMesh->renderInstanced(GL_TRIANGLES, shader, &waterPos[0], waterPos.size());

		}

		shader->disable();
	}

}

void World::renderAirplanes(Camera* camera) {
	Airplane* base;
	std::vector<Matrix44> enemiesPos;
	Mesh* airplaneMesh = NULL;
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");

	if (Airplane::airplanes.size() > 1) {
		base = Airplane::airplanes[1];
		airplaneMesh = base->getCorrectMeshRespectCameraDistance(camera);
	}

	for (int i = 1; i < Airplane::airplanes.size(); i++) {
		Airplane* airplane = Airplane::airplanes[i];
		//if (airplane->name.compare("player") != 0) {
			enemiesPos.push_back(airplane->getGlobalMatrix());
		//}
	}

	if (shader != NULL) {
		shader->enable();

		if (airplaneMesh != NULL) {
			shader->setUniform("u_color", base->material->color);
			shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
			shader->setUniform("u_texture", base->material->texture);
			shader->setUniform("u_camera_position", camera->eye);
			shader->setUniform("u_time", 1);

			airplaneMesh->renderInstanced(GL_TRIANGLES, shader, &enemiesPos[0], enemiesPos.size());

		}

		shader->disable();
	}

}

void World::render(Camera* camera) {
	if (this->sky != NULL) {
		this->sky->transform.matrixModel.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
		glDisable(GL_DEPTH_TEST);
		this->sky->render(camera);
		glEnable(GL_DEPTH_TEST);
	}

	if (this->getWorldMap() != NULL) {
		this->renderWorldMap(camera);
	}

	if (this->player != NULL) {
		this->player->render(camera);
	}

	this->renderAirplanes(camera);
	BulletManager::instance->render();
}

void World::update(float deltaTime) {

	if (this->root != NULL) {
		this->root->update(deltaTime);
	}

	if (this->player != NULL) {

		this->player->update(deltaTime);

		Vector3 cameraPosition = this->player->transform.matrixModel * Vector3(0, 3, 9);
		Vector3 cameraCenter = this->player->transform.matrixModel * (this->player->highMesh->box.center + Vector3(0, 0, 1));
		Vector3 cameraUp = this->player->transform.matrixModel.rotateVector(Transform::UP);
		playerCamera->lookAt(cameraPosition, cameraCenter, cameraUp);

	}

	if (this->sky != NULL) {
		this->sky->update(deltaTime);
	}

	BulletManager::instance->update(deltaTime);

}

Entity* World::getWorldMap() {
	bool founded = false;
	Entity* worldMap = NULL;
	for (int i = 0; (i < this->root->children.size()) && (founded == false); i++) {
		Entity* child = this->root->children[i];
		if (child->name.compare("worldMap") == 0) {
			worldMap = child;
			founded = true;
		}
	}
	return worldMap;
}
