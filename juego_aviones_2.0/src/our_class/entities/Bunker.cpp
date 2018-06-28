#include "Bunker.h"
//#include "CollisionHandler.h"
#include "../World.h"

#include "../weapons/Bullet.h"
#include "../managers/SoundManager.h"
#include "../stages/EndStage.h"
#include "Airplane.h"

// --- CONSTRUCTORES ---

Bunker::Bunker(const Transform transform, Mesh * highMesh, Material * material, char team) : EntityCollider(transform, highMesh, material) {
	
	this->health = 1000;
	this->state = BUNKER_LIVING;
	this->team = team;
	this->type = MILITARY_BASE;

}

Bunker::Bunker(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material, char team) : EntityCollider(transform, highMesh, lowMesh, material) {

	this->health = 1000;
	this->state = BUNKER_LIVING;
	this->team = team;
	this->type = MILITARY_BASE;
}

Bunker::~Bunker() {
	delete this;
}


void Bunker::render(Camera* camera) {
	EntityMesh::render(camera);
}

void Bunker::update(float deltaTime) {

	if (this->state != BUNKER_DESTROYED) {

		EntityCollider::update(deltaTime);

	}

}

void Bunker::onBulletCollision(Bullet & bullet, Vector3 collision) {

	std::cout << "SOME ONE HIT BUNKER!!!!!!!! " << std::endl;
	Vector3 globalCollision = this->getGlobalMatrix() * collision;

	std::cout << "Collision Y !!!!!!!! " << globalCollision.y << std::endl;

	this->health -= bullet.damage;

	if (this->health <= 0) {

		std::cout << "I DESTROY THE BUNKER!!!!!! " << std::endl;
		this->state = BUNKER_DESTROYED;

		if (this->team != World::instance->player->team) {
			EndStage::instance->win();
		} else {
			EndStage::instance->lose();
		}

	}

	//SoundManager::reproduceSound("damage.wav");

}

void Bunker::collisionEffectAgainstDynamicEntity() {
	//this->material->color = Vector4(0, 0, 0, 1);
	//this->state = AIRPLANE_DESTROYED;
	//airplanesToDestroy.push_back(this);
}