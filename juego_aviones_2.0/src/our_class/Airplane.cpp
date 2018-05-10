#include "Airplane.h"
#include "../input.h"
//#include "CollisionHandler.h"
#include "World.h";

// --- CONSTRUCTORES ---

Airplane::Airplane(float speed, const Vector3 position) : EntityMesh(position) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh) : EntityMesh(position, highMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh) : EntityMesh(position, rotation, highMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color) : EntityMesh(position, highMesh, texture, shader, color) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh,
					Texture* texture, Shader* shader, Vector4 color) : EntityMesh(position, rotation, highMesh, texture, shader, color) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh,
					Material * material) : EntityMesh(position, rotation, highMesh, material) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Material * material) : EntityMesh(transform, highMesh, material) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh) : EntityMesh(position, highMesh, lowMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh)
					: EntityMesh(position, rotation, highMesh, lowMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color)
					: EntityMesh(position, highMesh, lowMesh, texture, shader, color) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
					Texture* texture, Shader* shader, Vector4 color) : EntityMesh(position, rotation, highMesh, lowMesh, texture, shader, color) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
					Material * material) : EntityMesh(position, rotation, highMesh, lowMesh, material) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
					: EntityMesh(transform, highMesh, lowMesh, material) {
	this->speed = speed;
	this->health = 100;
}

Airplane::~Airplane() {}

void Airplane::render(Camera* camera) { 
	EntityMesh::render(camera);
	this->weapons[currentWepon]->render();
}

void Airplane::update(float deltaTime) {

	Vector3 collision;
	Vector3 normal;

	float deltaMove = deltaTime * this->speed * 0.05;

	this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

	if (this->name.compare("player") == 0) {
		this->rotateAirplane(deltaMove);
		this->turbo(deltaTime);
		this->shoot();
		this->weapons[currentWepon]->update(deltaTime);

		if (this->detectCollision(collision, normal) == true) {
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
	if (Input::isKeyPressed(SDL_SCANCODE_SPACE) == true) {
		this->weapons[currentWepon]->shoot(this->transform.matrixModel);
	}
}

bool Airplane::detectCollision(Vector3 & collisionPoint, Vector3 & normal) {
	Vector3 origin = this->getGlobalPosition();
	Vector3 direction = Vector3(0, 0, -1);
	std::vector<Entity*> a = World::instance->worldMap->children;
	return this->highMesh->testRayCollision(a[0]->transform.matrixModel, origin, direction, collisionPoint, normal);
	//return CollisionHandler::haveAnyRayCollision(origin, direction, a, collisionPoint, normal);
}