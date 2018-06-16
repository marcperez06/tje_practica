#include "Entity.h"

Entity::Entity() {
	this->parent = NULL;
	this->name = "";
	this->uuid = -1;
	this->type = ENTITY;
}

Entity::Entity(const Vector3 position) {
	this->parent = NULL;
	this->transform = Transform(position, Quaternion());
	this->name = "";
	this->uuid = -1;
	this->type = ENTITY;
}

Entity::Entity(const Vector3 position, const Quaternion rotation) {
	this->parent = NULL;
	this->transform = Transform(position, rotation);
	this->name = "";
	this->uuid = -1;
	this->type = ENTITY;
}

Entity::Entity(const Transform transform) {
	this->parent = NULL;
	this->transform = transform;
	this->name = "";
	this->uuid = -1;
	this->type = ENTITY;
}

Entity::~Entity() {
	if (this->parent != NULL) { this->parent->removeChild(this); }
}

void Entity::moveTo(const Vector3 pos) {
	Matrix44 newModel;
	Vector3 newPos = newModel * pos;
	this->transform.translate(newPos);
}

Vector3 Entity::getPosition() {
	return this->transform.matrixModel * Vector3();
}

Vector3 Entity::getGlobalPosition() {
	return this->getGlobalMatrix() * Vector3();
}

Matrix44 Entity::getGlobalMatrix() {
	
	if (this->parent == NULL) {
		return this->transform.matrixModel;
	} else {
		return this->transform.matrixModel * this->parent->getGlobalMatrix();
	}

}

void Entity::render(Camera* camera) {

	Matrix44 globalMatrix = this->getGlobalMatrix();

	this->renderMesh(camera, globalMatrix);

	for (int i = 0; i < this->children.size(); i++) {
		if (this->children[i] != NULL) {
			this->children[i]->render(camera);
		}
	}
}

void Entity::update(float deltaTime) {
	for (int i = 0; i < this->children.size(); i++) {
		if (this->children[i] != NULL) {
			this->children[i]->update(deltaTime);
		}
	}
}

void Entity::renderMesh(Camera* camera, Matrix44 globalMatrix) {}

void Entity::addChild(Entity* entity) {
	if (entity != NULL) {
		entity->parent = this;
		this->children.push_back(entity);
	}
}

void Entity::removeChild(Entity* entity) {

	for (int i = 0; i < this->children.size(); i++) {
		if (this->children[i] == entity) {
			this->children.erase(this->children.begin() + i);
			this->children[i]->parent = NULL;
		}
	}
}

Vector3 Entity::getFront() {
	return this->getGlobalMatrix().rotateVector(Vector3(0, 0, -1));
}

/*
bool Entity::haveRayCollision(Vector3 origin, Vector3 direction) {
	this->detectRayCollision(origin, direction);
	return this->collision.haveCollision;
}

void Entity::detectRayCollision(Vector3 origin, Vector3 direction) {
	for (int i = 0; i < this->children.size(); i++) {
		if (this->children[i] != NULL) {
			this->children[i]->detectRayCollision(origin, direction);
			if (this->children[i]->collision.haveCollision == true) {

				this->children[i]->parent->collision.origin = origin;
				this->children[i]->parent->collision.direction = direction;
				this->children[i]->parent->collision.collisionPoint = this->children[i]->collision.collisionPoint;
				this->children[i]->parent->collision.normalPoint = this->children[i]->collision.normalPoint;
				this->children[i]->parent->collision.haveCollision = true;

			}
		}
	}
}
*/