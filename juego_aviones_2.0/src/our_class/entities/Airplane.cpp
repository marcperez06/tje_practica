#include "Airplane.h"
#include "../../input.h"
//#include "CollisionHandler.h"
#include "../World.h"
#include "../managers/BulletManager.h"
#include "../weapons/Weapon.h"
#include "../controllers/AirplaneController.h"
#include "../managers/SoundManager.h"
#include "../stages/EndStage.h"
#include "ParticleSystem.h"

// --- CONSTRUCTORES ---

std::vector<Airplane*> Airplane::airplanes;
std::vector<Airplane*> Airplane::airplanesToDestroy;

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Material * material) : EntityCollider(transform, highMesh, material) {
	this->speed = speed;
	this->health = 100;
	this->fuell = 500;
	this->state = AIRPLANE_FLYING;
	this->target = NULL;
	this->isPlayer = false;
	this->team = TEAM_DELTA;
	this->type = AIRPLANE;
	this->particleSystem = new ParticleSystem();
	airplanes.push_back(this);
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
					: EntityCollider(transform, highMesh, lowMesh, material) {
	this->speed = speed;
	this->health = 100;
	this->fuell = 500;
	this->state = AIRPLANE_FLYING;
	this->target = NULL;
	this->isPlayer = false;
	this->team = TEAM_DELTA;
	this->type = AIRPLANE;
	this->particleSystem = new ParticleSystem();
	airplanes.push_back(this);
}

Airplane::~Airplane() {
	delete this->particleSystem; 
	delete this->controller;
	this->removeAirplane(this);
	this->removeAirplaneToDestroy(this);
	this->removeWeapons();
}

void Airplane::removeAirplane(Airplane* airplane) {
	bool founded = false;
	for (int i = 0; (i < airplanes.size()) && (founded == false); i++) {
		if (airplanes[i] == airplane) {
			airplanes.erase(airplanes.begin() + i);
			airplanesToDestroy.push_back(airplane);
			founded = true;
		}
	}
}

void Airplane::removeAirplaneToDestroy(Airplane* airplane) {
	bool founded = false;
	for (int i = 0; (i < airplanesToDestroy.size()) && (founded == false); i++) {
		if (airplanesToDestroy[i] == airplane) {
			airplanesToDestroy.erase(airplanes.begin() + i);
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
	
	if (this->particleSystem != NULL && this->particleSystem->particlesSize() > 0) {
		glDisable(GL_DEPTH_TEST);
		this->particleSystem->render(camera);
		glEnable(GL_DEPTH_TEST);
	}

	#ifndef DEBUG

	Mesh m;
	for (int i = 0; i < path.wayPoints.size(); ++i) {
		m.vertices.push_back(path.wayPoints[i]->getPosition());
	}
	if (m.vertices.size()>0) m.renderFixedPipeline(GL_LINE_STRIP);

	#endif
}

void Airplane::update(float deltaTime) {

	if (this->state != AIRPLANE_DESTROYED) {

		EntityCollider::update(deltaTime);

		if (this->state == AIRPLANE_CRASHED) {
			this->transform.translate(Vector3(0, -1, -1) * this->speed * deltaTime);
		} else {

			this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

			this->controller->update(deltaTime);

			this->material->color = Vector4(1, 1, 1, 1);

			this->health = 45;
			Bullet b;
			onBulletCollision(b, collision.collisionPoint);

			if (this->particleSystem != NULL) {
				this->particleSystem->update(deltaTime, this->getGlobalMatrix());
			}

			if (this->isPlayer == true) {

				this->fuell -= deltaTime;

				if (this->fuell < 60) {
					SoundManager::reproduceSound("low_fuel.wav");
				}

				if (this->fuell <= 0) {
					this->state = AIRPLANE_CRASHED;
				}
			}

		}

		if (this->getPosition().y < -100) {
			this->state = AIRPLANE_DESTROYED;
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
	this->transform.matrixModel.translate(0, 0, -5 * this->speed * deltaTime);
}

void Airplane::selectWeapon(char type) {
	boolean founded = false;
	
	for (int i = 0; i < this->weapons.size() && founded == false; i++) {
		if (weapons[i]->type == type) {
			founded = true;
			this->currentWepon = i;
		}
	}

	if (founded == false) {
		this->currentWepon = 0;
	}
}

Weapon* Airplane::getWeapon(char type) {
	Weapon* weapon = NULL;
	boolean founded = false;

	for (int i = 0; i < this->weapons.size() && founded == false; i++) {
		if (weapons[i]->type == type) {
			founded = true;
			weapon = this->weapons[i];
		}
	}
	return weapon;
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
	Vector3 globalCollision = this->getGlobalMatrix() * collision;

	std::cout << "Collision Y !!!!!!!! " << globalCollision.y << std::endl;

	this->health -= bullet.damage;

	if (this->isPlayer == true) {
		SoundManager::reproduceSound("damage.wav");
	}

	if (this->health <= 50) {
		if (this->particleSystem != NULL && this->particleSystem->particlesSize() == 0) {
			delete this->particleSystem;
			this->particleSystem = ParticleSystem::createSmoke(this->getGlobalMatrix());
		
		}
	}

	if (this->health <= 0) {
		this->state = AIRPLANE_CRASHED;
		World::instance->createRandomPowerup();
		SoundManager::reproduceSound("killingAirplane.wav");
		if (this->particleSystem != NULL) {
			delete this->particleSystem;
			this->particleSystem = ParticleSystem::createExplosion(this->getGlobalMatrix());
		}

		if (this->isPlayer == true) {
			EndStage::instance->lose();
		}
	}

	Mesh mesh;
	mesh.vertices.push_back(this->getGlobalMatrix() * collision);
	mesh.colors.push_back(Vector4(0, 0, 1, 1));
	glPointSize(500);
	mesh.renderFixedPipeline(GL_POINTS);
}

void Airplane::collisionEffectAgainstStaticEntity() {
	this->material->color = Vector4(0, 0, 0, 1);
	this->state = AIRPLANE_DESTROYED;
	if (this->isPlayer == true) {
		EndStage::instance->lose();
	}
	//airplanesToDestroy.push_back(this);
}

void Airplane::collisionEffectAgainstDynamicEntity() {
	this->material->color = Vector4(0, 0, 0, 1);
	this->state = AIRPLANE_CRASHED;
	if (this->isPlayer == true) {
		EndStage::instance->lose();
	}
	//airplanesToDestroy.push_back(this);
}

void Airplane::destroyDeadAirplanes() {
	for (int i = 0; i < airplanesToDestroy.size(); i++) {
		if (airplanesToDestroy[i]->state == AIRPLANE_DESTROYED) {
			delete airplanesToDestroy[i];
		}
	}
}