#include "AirplaneController.h"
#include "AIController.h"
#include "EntityCollider.h"
#include "World.h"
#include "Airplane.h"

AIController::AIController() {
	this->state = AIRPLANE_FLYING;
}
AIController::~AIController() {}


void AIController::render() {}

void AIController::update(float deltaTime) {

	if (this->state != AIRPLANE_CRHASED) {
		checkStateEnemy();
	}
	else if (this->state == AIRPLANE_CRHASED) {
		//Eliminar mesh i cridar destructor de Airplane
	}

}

void AIController::checkStateEnemy() {
	std::vector<Airplane*> enemies = World::instance->enemies;
	Airplane* player = World::instance->player;
	for (int i = 1; i < enemies.size(); i++) {
		Airplane* airplane = enemies[i];
		Vector3 positionEnemi = airplane->getPosition();
		if (positionEnemi.distance(player->getPosition())<150) {
			this->state = AIRPLANE_SHOOT;
		}
		else if (positionEnemi.distance(player->getPosition()) < 300) {
			this->state = AIRPLANE_FOLLOW;
		}
	}
}
