#include "AIController.h"
#include "World.h"
#include "Airplane.h"

AIController::AIController() : AirplaneController() {
	this->state = FLYING;
}

AIController::~AIController() {}

//void AIController::render() {}

void AIController::update(float deltaTime) {

	checkBehaviour();

	if (this->state != CRHASED) {
		checkStateEnemy();
	}
	else if (this->state == CRHASED) {
		//Eliminar mesh i cridar destructor de Airplane
	}

}

void AIController::checkBehaviour() {

}

void AIController::checkStateEnemy() {
	std::vector<Airplane*> enemies = World::instance->enemies;
	Airplane* player = World::instance->player;
	for (int i = 1; i < enemies.size(); i++) {
		Airplane* airplane = enemies[i];
		Vector3 positionEnemi = airplane->getPosition();
		if (positionEnemi.distance(player->getPosition())<150) {
			this->state = SHOOT;
		}
		else if (positionEnemi.distance(player->getPosition()) < 300) {
			this->state = FOLLOW;
		}
	}
}
