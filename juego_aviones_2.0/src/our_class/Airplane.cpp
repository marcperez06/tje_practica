#include "Airplane.h"
#include "../input.h"

// --- CONSTRUCTORES ---

Airplane::Airplane(float speed, const Vector3 position) : GameObject(position) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh) : GameObject(position, highMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh) : GameObject(position, rotation, highMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color) : GameObject(position, highMesh, texture, shader, color) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh,
					Texture* texture, Shader* shader, Vector4 color) : GameObject(position, rotation, highMesh, texture, shader, color) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh,
					Material * material) : GameObject(position, rotation, highMesh, material) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Material * material) : GameObject(transform, highMesh, material) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh) : GameObject(position, highMesh, lowMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh)
					: GameObject(position, rotation, highMesh, lowMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color)
					: GameObject(position, highMesh, lowMesh, texture, shader, color) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
					Texture* texture, Shader* shader, Vector4 color) : GameObject(position, rotation, highMesh, lowMesh, texture, shader, color) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
					Material * material) : GameObject(position, rotation, highMesh, lowMesh, material) {

	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
					: GameObject(transform, highMesh, lowMesh, material) {
	this->speed = speed;
	this->health = 100;
}

void Airplane::update(float deltaTime) {

	Vector3 up = this->transform.matrixModel.rotateVector(Vector3(0, 0, 1));
	Vector3 right = this->transform.matrixModel.rotateVector(Vector3(1, 0, 0));

	float deltaMove = deltaTime * this->speed * 0.3;

	this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

	// up
	if (Input::isKeyPressed(SDL_SCANCODE_W) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, right);
	}

	// down
	if (Input::isKeyPressed(SDL_SCANCODE_S) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, right);
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