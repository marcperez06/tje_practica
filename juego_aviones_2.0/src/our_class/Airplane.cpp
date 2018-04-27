#include "Airplane.h"

// --- CONSTRUCTORES ---

Airplane::Airplane(float speed, const Vector3 position) : GameObject(position) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh) : GameObject(position, highMesh) {
	this->speed = speed;
	this->health = 100;
}

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, const Quaternion rotation) : GameObject(position, rotation, highMesh) {
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

Airplane::Airplane(float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh, const Quaternion rotation)
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