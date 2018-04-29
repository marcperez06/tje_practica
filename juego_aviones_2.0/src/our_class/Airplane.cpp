#include "Airplane.h"
#include "../input.h"

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

void Airplane::render(Camera* camera) {}

void Airplane::update(float deltaTime) {
	
	float deltaMove = deltaTime * this->speed * 0.1;

	this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

	this->rotateAirplane(deltaMove);

}

void Airplane::rotateAirplane(float deltaMove) {
	
	Vector3 up = this->transform.matrixModel.rotateVector(Vector3(0, 0, 1)); // roll
	Vector3 front = this->transform.matrixModel.rotateVector(Vector3(1, 0, 0));  // jaw

	// up
	if (Input::isKeyPressed(SDL_SCANCODE_W) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, front);
	}

	// down
	if (Input::isKeyPressed(SDL_SCANCODE_S) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, front);
	}

	// right
	if (Input::isKeyPressed(SDL_SCANCODE_D) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, up);
	}

	// left
	if (Input::isKeyPressed(SDL_SCANCODE_A) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, up);
	}

}