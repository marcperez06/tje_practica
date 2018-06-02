#include "Airplane.h"
#include "../input.h"
//#include "CollisionHandler.h"
#include "World.h"
#include "BulletManager.h"
#include "Weapon.h"
#include "AirplaneController.h"

// --- CONSTRUCTORES ---

std::vector<Airplane*> Airplane::airplanes;

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Material * material) : EntityCollider(transform, highMesh, material) {
	this->speed = speed;
	this->health = 100;
	this->state = AIRPLANE_FLYING;
	this->target = NULL;
	this->isPlayer = false;
	this->team = TEAM_DELTA;
	this->type = AIRPLANE;
	airplanes.push_back(this);
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
					: EntityCollider(transform, highMesh, lowMesh, material) {
	this->speed = speed;
	this->health = 100;
	this->state = AIRPLANE_FLYING;
	this->target = NULL;
	this->isPlayer = false;
	this->team = TEAM_DELTA;
	this->type = AIRPLANE;
	airplanes.push_back(this);
}

Airplane::~Airplane() {
	this->removeAirplane(this);
	this->removeWeapons();
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

void Airplane::removeWeapons() {
	bool founded = false;
	for (int i = 0; i < this->weapons.size(); i++) {
		this->weapons.erase(this->weapons.begin() + i);
	}
}

void Airplane::render(Camera* camera) {
	EntityMesh::render(camera);
	for (int i = 0; i < this->weapons.size(); i++) {
		this->weapons[i]->render();
	}
}

void Airplane::update(float deltaTime) {

	if (this->state != AIRPLANE_CRHASED) {

		EntityCollider::update(deltaTime);

		this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

		this->controller->update(deltaTime);

		if (this->isPlayer == true) {
			/*if (this->detectStaticCollision() == true) {
				std::cout << "Collision !!" << std::endl;
				this->state = AIRPLANE_CRHASED;
			}*/
		}

	}

	for (int i = 0; i < this->weapons.size(); i++) {
		this->weapons[i]->update(deltaTime);
	}

}

void Airplane::rotateAirplane(float deltaMove) {
	this->rotateRollDirection(deltaMove);
	this->rotatePitchDirection(deltaMove);
	this->rotateYawDirection(deltaMove);
}

void Airplane::rotateRollDirection(float deltaMove) {
	Vector3 roll = Vector3(0, 0, 1);
	this->transform.matrixModel.rotate(deltaMove, roll);
}

void Airplane::rotatePitchDirection(float deltaMove) {
	Vector3 pitch = Vector3(1, 0, 0);
	this->transform.matrixModel.rotate(deltaMove, pitch);
}

void Airplane::rotateYawDirection(float deltaMove) {
	Vector3 yaw = Vector3(0, 1, 0);
	this->transform.matrixModel.rotate(deltaMove, yaw);
}

void Airplane::turbo(float deltaTime) {
	this->transform.translate(Vector3(0, 0, 10) * this->speed * deltaTime);
}

void Airplane::shoot() {	
	this->weapons[currentWepon]->shoot();
}

bool Airplane::detectStaticCollision() {
	bool haveCollision = false;
	Vector3 origin = this->lastPosition;
	Vector3 direction = this->getPosition() - origin;
	return EntityCollider::haveCollisionAgainstStaticObjects(origin, direction);
}

void Airplane::onBulletCollision(Bullet & bullet, Vector3 collision) {
	
	std::cout << "SOME ONE HIT ME!!!!!!!! " << std::endl;
	std::cout << "Collision Y !!!!!!!! " << collision.y << std::endl;

	this->health -= bullet.damage;
	if (this->health <= 0) {
		this->collisionEffect();
	}

	Mesh mesh;
	mesh.vertices.push_back(collision);
	mesh.colors.push_back(Vector4(0, 0, 1, 1));
	glPointSize(500);
	mesh.renderFixedPipeline(GL_POINTS);
}

void Airplane::collisionEffect() {
	this->material->color = Vector4(0, 0, 0, 1);
	this->state = AIRPLANE_CRHASED;
}