#include "AIController.h"
#include "World.h"
#include "Airplane.h"
#include "Path.h"

AIController::AIController() : AirplaneController() {
	this->state = FLYING;
}

AIController::~AIController() {}

void AIController::render() {}

void AIController::update(float deltaTime) {

	if (this->airplane->state != AIRPLANE_CRASHED && this->airplane->state != AIRPLANE_DESTROYED) {

		this->state = checkBehaviour();

		switch (this->state) {
			case FOLLOW:
				this->followTarget(deltaTime);
				break;
			case SHOOT:
				this->airplane->shoot();
				break;
			case EVADE:
				//this->evade();
				break;
			case ESCAPE:
				//this->escape();
				break;
			case PATROL:
				//this->patrol();
				break;
		}

		/*
		if (this->state != CRHASED) {
			checkStateEnemy();
		}
		else if (this->state == CRHASED) {
			//Eliminar mesh i cridar destructor de Airplane
		}

		*/

	}

}

char AIController::checkBehaviour() {

	if (this->airplane->health <= 10) {
		return ESCAPE;
	}

	std::vector<Airplane*> airplanes = Airplane::airplanes;
	for (int i = 0; i < airplanes.size(); i++) {

		if (this->airplane == airplanes[i] || airplanes[i]->state == AIRPLANE_CRASHED || airplanes[i]->state == AIRPLANE_DESTROYED) {
			continue;
		}

		selectTarget(airplanes[i]);
		if (World::isEntityANearEntityB(this->airplane, airplanes[i]) == true) {
			return EVADE;
		}

	}

	if (World::distanceBetween(this->airplane, this->airplane->target) < 350) {
		if (World::angleBetween(this->airplane, this->airplane->target) < 0.01) {
			return SHOOT;
		} else {
			return FOLLOW;
		}
	} else if (World::distanceBetween(this->airplane, this->airplane->target) < 1000
				|| World::distanceBetween(this->airplane, this->airplane->target) > 1500) {
		return FOLLOW;
	}
	

	/*
	std::vector<Entity*> islands = World::instance->getWorldMap()->children;
	for (int i = 0; i < islands.size(); i++) {


		if (this->airplane->getGlobalPosition().y > islands[i]->getGlobalPosition().y) {
			continue;
		}

		//selectTarget(airplanes[i]);
		if (World::isEntityANearEntityB(this->airplane, islands[i]) == true) {
			return EVADE;
		}

	}
	*/

}

void AIController::selectTarget(Entity* entity) {

	if (entity->type == AIRPLANE) {
		Airplane* target = (Airplane*) entity;
		if (target->state == AIRPLANE_CRASHED || target->state == AIRPLANE_DESTROYED
			|| target->team == this->airplane->team) {

			return;
		}
	}

	if (this->airplane->target != NULL) {
		if (this->airplane->target->type == AIRPLANE) {
			Airplane* target = (Airplane*) this->airplane->target;
			if (target->state == AIRPLANE_CRASHED || target->state == AIRPLANE_DESTROYED
				|| target->team == this->airplane->team) {
				this->airplane->target == NULL;
			}
		}
	}

	if (World::entityACanSeeEntityB(this->airplane, entity) == true) {
		if (this->airplane->target == NULL) {
			this->airplane->target = entity;
			return;
		}
	}

}

void AIController::checkStateEnemy() {
	std::vector<Airplane*> enemies = World::instance->AIAirplanes;
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

void AIController::followTarget(float deltaTime) {

	if (this->airplane->target == NULL) {
		return;
	}

	if (this->airplane->target->type == AIRPLANE) {
		Airplane* airplaneEntity = (Airplane*) this->airplane->target;
		if (airplaneEntity->health <= 0) {
			return;
		}
	}

	Matrix44 modelInverse = this->airplane->getGlobalMatrix();
	modelInverse.inverse();

	Vector3 targetFront = this->airplane->target->getFront();
	Vector3 myFront = this->airplane->getFront();

	targetFront.normalize();

	Vector3 pos = this->airplane->getPosition();
	Vector3 targetPos = this->airplane->target->getPosition();
	Vector3 toTarget = (targetPos - pos);

	std::cout << "POS " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	std::cout << "TO TARGET " << toTarget.x << " " << toTarget.y << " " << toTarget.z << std::endl;
	//std::cout << "TARGET TO POS " << targetPos.x << " " << targetPos.y << " " << targetPos.z << std::endl;

	if (abs(toTarget.length()) < 0.0001) {
		return;
	}

	toTarget.normalize();

	float cos_angle = 1.0 - myFront.dot(toTarget);
	Vector3 axis = toTarget.cross(myFront);

	axis = modelInverse.rotateVector(axis);
	this->airplane->transform.rotate(cos_angle * deltaTime * 3.5, axis);


	/*
	Matrix44 modelInverse = this->airplane->getGlobalMatrix();
	modelInverse.inverse();

	Vector3 pos = this->airplane->getGlobalPosition();
	Vector3 targetPos = this->airplane->target->getGlobalPosition();

	if (pos.y < 300) {
		//this->airplane->rotatePitchDirection(this->airplane->speed * 0.02);
		//Vector3 newTargetPos = pos + Transform::UP * 10;
		//targetPos = newTargetPos;
	}

	Vector3 toTarget = (targetPos - pos);

	std::cout << "POS " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	std::cout << "TO TARGET " << toTarget.x << " " << toTarget.y << " " << toTarget.z << std::endl;
	//std::cout << "TARGET TO POS " << targetPos.x << " " << targetPos.y << " " << targetPos.z << std::endl;

	if (abs(toTarget.length()) < 0.0001) {
		return;
	}

	toTarget.normalize();

	Vector3 front = this->airplane->getFront();

	if (abs(front.length()) < 0.0001) {
		return;
	}

	front.normalize();

	float frontDotToTarget = front.dot(toTarget);
	if (abs(frontDotToTarget) >= 1) {
		return;
	}

	float angle = acos(frontDotToTarget);

	if (abs(angle) < 0.01) {
		return;
	}

	Vector3 axis = front.cross(toTarget) * -1;
	axis = modelInverse.rotateVector(axis);
	axis.normalize();

	this->airplane->transform.rotate(angle * deltaTime, axis);
	*/
}