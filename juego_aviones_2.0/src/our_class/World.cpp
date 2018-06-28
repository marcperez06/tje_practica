#include "World.h"
#include "Factory.h"
#include <stdlib.h>
#include "../game.h";
#include "managers/BulletManager.h"
#include "managers/ProjectileManager.h"
#include "managers/CollisionHandler.h"
#include "entities/Airplane.h"
#include "weapons/MachineGun.h"
#include "weapons/DropBomb.h"
#include "weapons/RocketLauncher.h"
#include "weapons/ShootGun.h"
#include "controllers/PlayerController.h"
#include "controllers/AIController.h"
#include "controllers/AirplaneController.h"
#include "entities/Clouds.h"
#include "entities/Powerup.h"
#include "entities/Bunker.h"

#include "entities/ParticleSystem.h"

World* World::instance = NULL;

World::World(int hardFactor) {
	this->hardFactor = hardFactor;
	this->root = new Entity(Vector3(0, 0, 0));
	this->numAIAirplanes = 2;
	this->numOfTeams = 2;
	this->initPlayer();
	this->initBunkers();
	this->initCameras();
	this->initTeams();
	this->initWorldMap();
	this->initSky();
	this->initSea();
	this->clouds = new Clouds();

	this->createRandomPowerup();

	World::instance = this;
}

World::~World() {
	for (int i = 0; i < this->root->children.size(); i++) {
		delete this->root->children[i];
	}
	delete this->sky;
	delete this->sea;
	delete this->clouds;
	delete this->root;
	this->player = NULL;
	Airplane::airplanes.clear();
	Airplane::airplanesToDestroy.clear();
	Powerup::powerups.clear();
}

void World::initCameras() {
	Vector3 cameraPosition = this->player->transform.matrixModel * Vector3(0, 3, 9);
	//Vector3 cameraCenter = this->player->transform.matrixModel * (this->player->highMesh->box.center + Vector3(0, 0, 1));
	Vector3 cameraCenter = this->player->transform.matrixModel * this->player->getFront();
	Vector3 cameraUp = this->player->transform.matrixModel.rotateVector(Transform::UP);

	//create our free camera
	this->freeCamera = new Camera();
	this->freeCamera->lookAt(cameraPosition, cameraCenter, cameraUp); //position the camera and point to 0,0,0
	this->freeCamera->setPerspective(70.f, Game::instance->window_width / (float) Game::instance->window_height, 1, 30000.f); //set the projection, we want to be perspective

	this->playerCamera = new Camera();
	this->playerCamera->lookAt(cameraPosition, cameraCenter, cameraUp); //position the camera and point to 0,0,0
	this->playerCamera->setPerspective(70.f, Game::instance->window_width / (float) Game::instance->window_height, 1, 10000.f); //set the projection, we want to be perspective

	this->currentCamera = this->playerCamera;
}

void World::initPlayer() {
	this->player = Factory::buildAirplane(TEAM_ALFA, Vector3(-1565, 600, 13655), 205);
	this->player->fuell = this->player->fuell / this->hardFactor;
	this->player->name = "player";
	this->player->uuid = 1;
	this->player->path.createCircle(Vector3(-1565, 0, 13070), 600, 400);
	this->player->currentWepon = 0;
	this->player->isPlayer = true;
	this->player->controller = new PlayerController();
	this->player->controller->airplane = this->player;

	this->root->addChild(this->player);
	this->dynamicObjects.push_back(this->player);

}

void World::initBunkers() {

	Bunker* bunkerAlfa = Factory::buildBunker(TEAM_ALFA, Vector3(-1565, 0, 13070));
	this->teamMilitaryBases.push_back(bunkerAlfa);
	this->root->addChild(this->teamMilitaryBases[0]);
	this->dynamicObjects.push_back(this->teamMilitaryBases[0]);

	Bunker* bunkerDelta = Factory::buildBunker(TEAM_DELTA, Vector3(-815, 0, -12400));
	bunkerDelta->health = bunkerDelta->health * this->hardFactor;
	this->teamMilitaryBases.push_back(bunkerDelta);
	this->teamMilitaryBases[1]->transform.rotate(33 * DEG2RAD, Transform::UP);
	this->root->addChild(this->teamMilitaryBases[1]);
	this->dynamicObjects.push_back(this->teamMilitaryBases[1]);

}

void World::initTeams() {
	assert(this->player);
	int limit = this->numAIAirplanes / this->numOfTeams;
	if (this->player != NULL) {
		for (int i = 0; i < this->numAIAirplanes; i++) {

			if (i < limit) {
				initTeamAlfa();
			} else {
				initTeamDelta();
			}

		}

		// Bucle para seleccionar el target de cada avion.

		for (int i = 0; i < this->numAIAirplanes; i++) {

			int probabilityOfFollowPath = rand() % 100;
			char typeTarget = WAYPOINT; // (probabilityOfFollowPath > 60) ? WAYPOINT : MILITARY_BASE;
			int teamTarget = rand() % this->numOfTeams;

			teamTarget = (this->AIAirplanes[i]->team == teamTarget) ? (teamTarget + 1) % this->numOfTeams : teamTarget;

			if (typeTarget == MILITARY_BASE) {
				this->AIAirplanes[i]->target = this->teamMilitaryBases[teamTarget];
			} else if (typeTarget == WAYPOINT) {
				this->AIAirplanes[i]->target = this->AIAirplanes[i]->path.wayPoints[rand() % this->AIAirplanes[i]->path.wayPoints.size()];
			}

			// Seleccionar un avion enemigo aleatorio dependiendo de tu equipo

			/*
			if (i < limit) {
				int targetIndexOfAlfaTeam = limit + (rand() % numAIAirplanes - limit);
				this->AIAirplanes[i]->target = this->AIAirplanes[targetIndexOfAlfaTeam];
			} else if (i >= limit && i < limit * this->numOfTeams) {
				int aux = rand() % (limit * this->numOfTeams);
				int targetIndexOfDeltaTeam = (aux < limit) ? aux : limit * 2 + (rand() % (numAIAirplanes - limit * 2));
				this->AIAirplanes[i]->target = this->AIAirplanes[targetIndexOfDeltaTeam];
			} else if (i >= limit * 2 && i < limit * 3) {
				int aux = rand() % (limit * 3);
				int targetIndexOfBetaTeam = (aux < (limit * 2)) ? aux : limit * 3 + (rand() % (numAIAirplanes - limit * 3));
				this->AIAirplanes[i]->target = this->AIAirplanes[targetIndexOfBetaTeam];
			} else {
				int targetIndexOfGammaTeam = rand() % (numAIAirplanes - limit);
				this->AIAirplanes[i]->target = this->AIAirplanes[targetIndexOfGammaTeam];
			}
			*/

			//this->AIAirplanes[i]->target = this->player;
			
			/*
			Entity* auxEntity = new Entity(AIAirplanes[i]->getGlobalPosition());
			auxEntity->transform.translate(Vector3(100, 100, -100));
			AIAirplanes[i]->target = auxEntity;
			*/

		}
	}
}

void World::initTeamAlfa() {
	Vector3 bunkerPos = this->teamMilitaryBases[0]->getGlobalPosition();
	float x = (rand() % 50) + bunkerPos.x + this->player->highMesh->aabb_max.x;
	float y = (rand() % 100) + 600 + this->player->highMesh->aabb_max.y;
	float z = (rand() % 20) + bunkerPos.z + this->player->highMesh->aabb_max.z;

	Airplane* enemy = Factory::buildAirplane(TEAM_ALFA, Vector3(x, y, z), 200);
	enemy->health = enemy->health * this->hardFactor;
	enemy->path.createCircle(bunkerPos, 40, 200);
	enemy->isPlayer = false;

	enemy->controller = new AIController();
	enemy->controller->airplane = enemy;

	this->root->addChild(enemy);
	this->AIAirplanes.push_back(enemy);
	this->dynamicObjects.push_back(enemy);
}

void World::initTeamDelta() {
	Vector3 bunkerPos = this->teamMilitaryBases[1]->getGlobalPosition();
	float x = (rand() % 50) + bunkerPos.x + this->player->highMesh->aabb_max.x;
	float y = (rand() % 100) + 600 + this->player->highMesh->aabb_max.y;
	float z = (rand() % 20) + bunkerPos.z + this->player->highMesh->aabb_max.z;
	
	Airplane* enemy = Factory::buildAirplane(TEAM_DELTA, Vector3(x, y, z), 200);
	enemy->health = enemy->health * this->hardFactor;
	enemy->path.createCircle(bunkerPos, 600, 400);
	enemy->isPlayer = false;

	enemy->controller = new AIController();
	enemy->controller->airplane = enemy;

	this->root->addChild(enemy);
	this->AIAirplanes.push_back(enemy);
	this->dynamicObjects.push_back(enemy);
}

/*
void World::initTeamBeta() {
	Vector3 bunkerPos = this->teamMilitaryBases[2]->getGlobalPosition();
	float x = (rand() % 50) + bunkerPos.x + this->player->highMesh->aabb_max.x;
	float y = (rand() % 100) + 600 + this->player->highMesh->aabb_max.y;
	float z = (rand() % 20) + bunkerPos.z + this->player->highMesh->aabb_max.z;

	Airplane* enemy = Factory::buildAirplane(TEAM_BETA, Vector3(x, y, z), 200);
	enemy->health = enemy->health * this->hardFactor;
	enemy->path.createCircle(bunkerPos, 600, 400);
	enemy->isPlayer = false;

	enemy->controller = new AIController();
	enemy->controller->airplane = enemy;

	this->root->addChild(enemy);
	this->AIAirplanes.push_back(enemy);
	this->dynamicObjects.push_back(enemy);
}

void World::initTeamGamma() {
	Vector3 bunkerPos = this->teamMilitaryBases[3]->getGlobalPosition();
	float x = (rand() % 50) + bunkerPos.x + this->player->highMesh->aabb_max.x;
	float y = (rand() % 100) + 600 + this->player->highMesh->aabb_max.y;
	float z = (rand() % 20) + bunkerPos.z + this->player->highMesh->aabb_max.z;

	Airplane* enemy = Factory::buildAirplane(TEAM_GAMMA, Vector3(x, y, z), 200);
	enemy->health = enemy->health * this->hardFactor;
	enemy->path.createCircle(bunkerPos, 40, 200);
	enemy->isPlayer = false;

	enemy->controller = new AIController();
	enemy->controller->airplane = enemy;

	this->root->addChild(enemy);
	this->AIAirplanes.push_back(enemy);
	this->dynamicObjects.push_back(enemy);
}
*/

void World::initWorldMap() {
	Entity* worldMap = new Entity(Vector3(0, 0, 0));
	worldMap->name = "worldMap"; // Podem triar un nom..
	worldMap->uuid = 2;

	Mesh * meshIsland = Mesh::Load("data/island/island.ASE");
	float maxX = (meshIsland->aabb_max.x * 2) - 5;
	float maxZ = (meshIsland->aabb_max.z * 2) - 5;
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

void World::initSea() {
	assert(this->playerCamera);
	Vector3 initialPos = this->playerCamera->eye;
	initialPos.y = -100;
	this->sea = Factory::buildSea(initialPos);
	this->staticObjects.push_back(this->sea);
}

void World::render(Camera* camera) {
	if (this->sky != NULL) {
		this->sky->transform.matrixModel.setTranslation(camera->eye.x, camera->eye.y - 40, camera->eye.z);
		glDisable(GL_DEPTH_TEST);
		this->sky->render(camera);
		glEnable(GL_DEPTH_TEST);
	}

	if (this->sea != NULL) {
		this->sea->transform.matrixModel.setTranslation(camera->eye.x, -100, camera->eye.z);
		this->sea->render(camera);
	}

	if (this->getWorldMap() != NULL) {
		this->renderWorldMap(camera);
	}

	if (this->player != NULL) {
		this->player->render(camera);
	}

	this->renderAirplanes(camera);

	this->renderBunkers(camera);

	this->clouds->render(camera);

	this->renderPowerups(camera);

	BulletManager::instance->render();
	ProjectileManager::instance->render();
}

void World::renderWorldMap(Camera* camera) {
	EntityMesh* base;
	std::vector<Matrix44> islandsPos;

	Mesh* islandMesh = Mesh::Load("data/island/island.ASE");
	//Mesh* waterMesh = Mesh::Load("data/island/water_deep.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/world.fs");

	if (islandMesh == NULL || shader == NULL) {
		return;
	}

	if (this->getWorldMap()->children.size() > 0) {
		base = (EntityMesh*) this->getWorldMap()->children[0];
		//islandMesh = base->getCorrectMeshRespectCameraDistance(camera);
	}

	for (int i = 0; i < this->getWorldMap()->children.size(); i++) {
		EntityMesh* island = (EntityMesh*) this->getWorldMap()->children[i];
		islandsPos.push_back(island->getGlobalMatrix());
	}

	shader->enable();
	shader->setUniform("u_color", base->material->color);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", base->material->texture);
	shader->setUniform("u_detail_texture", Texture::Load("data/island/rock.tga"));
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_time", 1);

	islandMesh->renderInstanced(GL_TRIANGLES, shader, &islandsPos[0], islandsPos.size());

	shader->setUniform("u_texture", Texture::Load("data/island/water_deep.tga"));

	//waterMesh->renderInstanced(GL_TRIANGLES, shader, &islandsPos[0], islandsPos.size());

	shader->disable();

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
	std::vector<Matrix44> airplanesTransform;
	Mesh* airplaneMesh = NULL;
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");

	for (int i = 1; i < Airplane::airplanes.size(); i++) {
		Airplane* airplane = Airplane::airplanes[i];
		airplanesTransform.push_back(airplane->getGlobalMatrix());
	}

	if (airplanesTransform.size() > 0) {

		int limit = this->numAIAirplanes / this->numOfTeams;

		if (shader != NULL) {

			for (int i = 0; i < this->numOfTeams; i++) {

				if (Airplane::airplanes.size() > 0) {
					base = Airplane::airplanes[i * limit];
					airplaneMesh = base->getCorrectMeshRespectCameraDistance(camera);
				}

				shader->enable();

				if (airplaneMesh != NULL) {
					shader->setUniform("u_color", base->material->color);
					shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
					shader->setUniform("u_texture", base->material->texture);
					shader->setUniform("u_camera_position", camera->eye);
					shader->setUniform("u_time", 1);

					airplaneMesh->renderInstanced(GL_TRIANGLES, shader, &airplanesTransform[i * limit], airplanesTransform.size() - limit * (this->numOfTeams - 1));

					/* Si se eliminan los aviones, luego hay problema a la hora de renderizar, porque los valores cambian..
					if (numAIAirplanes == airplanesTransform.size()) {
						airplaneMesh->renderInstanced(GL_TRIANGLES, shader, &airplanesTransform[i * limit], airplanesTransform.size() - limit * 3);
					} else {
						int dif = numAIAirplanes - airplanesTransform.size();
						float range = airplanesTransform.size() - dif;
						airplaneMesh->renderInstanced(GL_TRIANGLES, shader, &airplanesTransform[i * limit], range - limit - dif * 3);
					}
					*/

				}

				shader->disable();
			}
		}

	}

}

void World::renderBunkers(Camera* camera) {

	for (int i = 0; i < teamMilitaryBases.size(); i++) {

		this->teamMilitaryBases[i]->render(camera);

	}

}

void World::renderPowerups(Camera* camera) {
	std::vector<Powerup*> powerups = Powerup::powerups;
	for (int i = 0; i < powerups.size(); i++) {
		if (powerups[i] != NULL) {
			powerups[i]->render(camera);
		}
	}
}

void World::update(float deltaTime) {

	/*
	if (this->root != NULL) {
		this->root->update(deltaTime);
	}
	*/

	for (int i = 0; i < this->dynamicObjects.size(); i++) {
		this->dynamicObjects[i]->update(deltaTime);
	}

	for (int i = 0; i < this->staticObjects.size(); i++) {
		this->staticObjects[i]->update(deltaTime);
	}

	if (this->AIAirplanes.size() > 0) {
		
		//this->cameraFollowEntity(this->playerCamera, AIAirplanes[0]);
		
	}

	this->cameraFollowEntity(this->playerCamera, this->player);

	this->updatePowerups(deltaTime);

	BulletManager::instance->update(deltaTime);
	ProjectileManager::instance->update(deltaTime);
	CollisionHandler::collisionStaticEntitesAgainstDynamicEntiteis();
	CollisionHandler::collisionDynamicEntitesAgainstDynamicEntiteis();

	Airplane::destroyDeadAirplanes();
}

void World::cameraFollowEntity(Camera* camera, Entity* entity) {
	Vector3 cameraPosition = entity->transform.matrixModel * Vector3(0, 3, 9);
	Vector3 cameraCenter = entity->transform.matrixModel * Vector3(0, 0, 0);
	if (entity->type != ENTITY) {
		//cameraCenter = entity->transform.matrixModel * ((EntityMesh*)entity)->highMesh->box.center + Vector3(0, 0, 1);
		cameraCenter = entity->transform.matrixModel * entity->getFront();
	}
	Vector3 cameraUp = entity->transform.matrixModel.rotateVector(Transform::UP);
	camera->lookAt(cameraPosition, cameraCenter, cameraUp);
}

void World::updatePowerups(float deltaTime) {
	std::vector<Powerup*> powerups = Powerup::powerups;
	for (int i = 0; i < powerups.size(); i++) {
		if (powerups[i] != NULL) {
			powerups[i]->update(deltaTime);
		}
	}
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

void World::createRandomPowerup() {
	PowerupType type = PowerupType(rand() % 5);
	this->createPowerup(type);
}

void World::createPowerup(char type) {
	Vector3 pos;
	pos.random(Vector3(40, 40, 0));
	pos = pos + this->player->getGlobalPosition() + Vector3(0, 0, -450);

	Powerup* powerup = new Powerup(type, pos);
}

// Funciones Utiles

bool World::entityACanSeeEntityB(Entity* entityA, Entity* entityB) {
	float angleOfview = 45 / 2.0;
	bool canSee = false;
	assert(entityA && entityB);

	Matrix44 modelInverse = entityA->getGlobalMatrix();
	modelInverse.inverse();

	Vector3 entityAPos = entityA->getGlobalPosition();
	Vector3 entityBPos = entityB->getGlobalPosition();

	Vector3 toEntityB = (entityBPos - entityAPos);

	if (abs(toEntityB.length()) < 0.0001) {
		return false;
	}

	toEntityB.normalize();

	Vector3 front = entityA->getGlobalMatrix().rotateVector(Vector3(0, 0, -1));

	if (abs(front.length()) < 0.0001) {
		return false;
	}

	front.normalize();

	float frontDotToEntity = front.dot(toEntityB);
	if (abs(frontDotToEntity) >= 1) {
		return false;
	}

	float angle = acos(frontDotToEntity);

	if (abs(angle) <= angleOfview) {
		canSee = true;
	}
	else {
		canSee = false;
	}

	return canSee;
}

bool World::isEntityANearEntityB(Entity* entityA, Entity* entityB) {
	bool isEntityBNear = false;

	assert(entityA && entityB);

	float radius = 50;
	float distance = World::distanceBetween(entityA, entityB);

	if (abs(distance) < radius) {
		isEntityBNear = true;
	} else {
		isEntityBNear = false;
	}

	return isEntityBNear;
}

float World::distanceBetween(Entity* entityA, Entity* entityB) {
	assert(entityA && entityB);

	Vector3 entityAPos = entityA->getGlobalPosition();
	Vector3 entityBPos = entityB->getGlobalPosition();

	return entityAPos.distance(entityBPos);
}

Vector3 World::directionEntityAToEntityB(Entity* entityA, Entity* entityB) {
	assert(entityA && entityB);

	Vector3 entityAPos = entityA->getGlobalPosition();
	Vector3 entityBPos = entityB->getGlobalPosition();

	Vector3 toEntityB = (entityAPos - entityBPos);
	return toEntityB;
}


float World::angleBetween(Entity* entityA, Entity* entityB) {
	float angle = 0;
	assert(entityA && entityB);

	Matrix44 modelInverse = entityA->getGlobalMatrix();
	modelInverse.inverse();

	Vector3 entityAPos = entityA->getGlobalPosition();
	Vector3 entityBPos = entityB->getGlobalPosition();

	Vector3 toEntityB = (entityBPos - entityAPos);

	if (abs(toEntityB.length()) < 0.0001) {
		return angle;
	}

	toEntityB.normalize();

	Vector3 front = entityA->getGlobalMatrix().rotateVector(Vector3(0, 0, -1));

	if (abs(front.length()) < 0.0001) {
		return angle;
	}

	front.normalize();

	float frontDotToEntity = front.dot(toEntityB);
	if (abs(frontDotToEntity) >= 1) {
		return angle;
	}

	angle = acos(frontDotToEntity);

	return angle;
}

template<class T>
void World::removeElement(std::vector<T> list, T element) {
	bool founded = false;
	for (int i = 0; (i < list.size()) && (founded == false); i++) {
		if (list[i] == element) {
			list.erase(list.begin() + i);
			founded = true;
		}
	}
}

template<class T>
void World::deleteAndRemoveElement(std::vector<T> list, T element) {
	bool founded = false;
	for (int i = 0; (i < list.size()) && (founded == false); i++) {
		if (list[i] == element) {
			list.erase(list.begin() + i);
			founded = true;
			delete element;
		}
	}
}