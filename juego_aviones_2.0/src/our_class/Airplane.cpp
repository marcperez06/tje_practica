#include "Airplane.h"
#include "../input.h"
//#include "CollisionHandler.h"
#include "World.h";

// --- CONSTRUCTORES ---

std::vector<Airplane*> Airplane::airplanes;

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Material * material) : EntityMesh(transform, highMesh, material) {
	this->speed = speed;
	this->health = 100;
	airplanes.push_back(this);
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
					: EntityMesh(transform, highMesh, lowMesh, material) {
	this->speed = speed;
	this->health = 100;
	airplanes.push_back(this);
}

Airplane::~Airplane() {
	this->removeAirplane(this);
}

void Airplane::render(Camera* camera) { 
	EntityMesh::render(camera);
	this->weapons[currentWepon]->render();
}

void Airplane::update(float deltaTime) {

	float deltaMove = deltaTime * this->speed * 0.02;

	this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

	if (this->name.compare("player") == 0) {
		this->rotateAirplane(deltaMove);
		this->turbo(deltaTime);
		this->shoot();
		this->weapons[currentWepon]->update(deltaTime);

		if (this->detectCollision() == true) {
			this->speed = 0;
		}

	}

}

void Airplane::rotateAirplane(float deltaMove) {
	this->rotateRollDirection(deltaMove);
	this->rotatePitchDirection(deltaMove);
	this->rotateJawDirection(deltaMove);
}

void Airplane::rotateRollDirection(float deltaMove) {
	Vector3 roll = Vector3(0, 0, 1);
	// roll right
	if (Input::isKeyPressed(SDL_SCANCODE_Q) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, roll);
	}

	// roll left
	if (Input::isKeyPressed(SDL_SCANCODE_E) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, roll);
	}
}

void Airplane::rotatePitchDirection(float deltaMove) {
	Vector3 pitch = Vector3(1, 0, 0);
	// down
	if (Input::isKeyPressed(SDL_SCANCODE_W) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, pitch);
	}

	// up
	if (Input::isKeyPressed(SDL_SCANCODE_S) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, pitch);
	}
}

void Airplane::rotateJawDirection(float deltaMove) {
	Vector3 jaw = Vector3(0, 1, 0);
	// jaw right
	if (Input::isKeyPressed(SDL_SCANCODE_D) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, jaw);
	}

	// jaw left
	if (Input::isKeyPressed(SDL_SCANCODE_A) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, jaw);
	}
}

void Airplane::turbo(float deltaTime) {
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) == true) {
		this->transform.translate(Vector3(0, 0, -10) * this->speed * deltaTime);
	}

	if (Input::isKeyPressed(SDL_SCANCODE_TAB) == true) {
		this->transform.translate(Vector3(0, 0, 10) * this->speed * deltaTime);
	}
}

void Airplane::shoot() {
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) == true) {
		this->weapons[currentWepon]->shoot(this->getGlobalMatrix() );
	}
}

bool Airplane::detectCollision() {
	bool haveCollision = false;
	Vector3 origin = this->getGlobalPosition();
	Vector3 direction = Vector3(0, 0, -1);

	for (int i = 0; (i < World::instance->root->children.size()) && (haveCollision == false); i++) {
		Entity* child = World::instance->root->children[i];
		haveCollision = child->haveRayCollision(origin, direction);
	}
	
	/*
	std::vector<Entity*> islands = World::instance->root->children;
	for (int i = 0; (i < islands.size()) && (haveCollision == false); i++) {
		EntityMesh* island = (EntityMesh*) islands[i];
		//haveCollision = island->highMesh->testRayCollision(island->getGlobalMatrix(), origin, direction, collisionPoint, normal);
	}
	*/
	return haveCollision;
}

void Airplane::removeAirplane(Airplane* airplane) {
	bool founded = false;
	for (int i = 0; (i < airplanes.size()) && (founded == false); i++) {
		if (airplanes[i] == airplane) {
			airplanes.erase(airplanes.begin() + i);
			founded = true;
		}
	}
}