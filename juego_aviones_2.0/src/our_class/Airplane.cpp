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

void Airplane::render(Camera* camera) { EntityMesh::render(camera); }

void Airplane::update(float deltaTime) {
	
	float deltaMove = deltaTime * this->speed * 0.05;

	this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

	if (this->name.compare("player") == 0) {
		this->rotateAirplane(deltaMove);
		this->turbo(deltaTime);
	}

}

void Airplane::rotateAirplane(float deltaMove) {
	
	Vector3 pitch = Vector3(1, 0, 0);
	//Vector3 jaw = Vector3(0, 1, 0);
	Vector3 roll = Vector3(0, 0, 1);

	// down
	if (Input::isKeyPressed(SDL_SCANCODE_W) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, pitch);
	}

	// up
	if (Input::isKeyPressed(SDL_SCANCODE_S) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, pitch);
	}

	// roll right
	if (Input::isKeyPressed(SDL_SCANCODE_A) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, roll);
	}

	// roll left
	if (Input::isKeyPressed(SDL_SCANCODE_D) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, roll);
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