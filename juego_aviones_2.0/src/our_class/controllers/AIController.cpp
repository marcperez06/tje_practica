#include "AIController.h"
#include "../World.h"
#include "../entities/Airplane.h"
#include "../entities/Path.h"

AIController::AIController() : AirplaneController() {
	this->firstTarget = NULL;
	this->state = FLYING;
}

AIController::AIController(Airplane* airplane) : AirplaneController(airplane) {
	this->firstTarget = this->airplane->target;
	this->state = FLYING;
}

AIController::~AIController() {}

void AIController::render() {
	Mesh m;
	for (int i = 0; i < airplane->path.wayPoints.size(); ++i) {
		m.vertices.push_back(airplane->path.wayPoints[i]->getGlobalPosition());
	}

	if (m.vertices.size()>0) m.renderFixedPipeline(GL_LINE_STRIP);
}

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
			case PATROL:
				this->patrol(deltaTime);
				break;
		  /*case EVADE:
				//this->evade();
				break;
			case ESCAPE:
				//this->escape();
				break;*/
		}

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
		/*
		if (World::isEntityANearEntityB(this->airplane, airplanes[i]) == true) {
			return EVADE;
		}
		*/

	}

	if (this->airplane->target->type != WAYPOINT) {
		if (World::distanceBetween(this->airplane, this->airplane->target) < 500) {
			if (World::angleBetween(this->airplane, this->airplane->target) < 0.05) {
				return SHOOT;
			} else {
				return FOLLOW;
			}
		} else {
			return FOLLOW;
		}
	} else {
		return PATROL;
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
		if (target->state == AIRPLANE_CRASHED || target->state == AIRPLANE_DESTROYED || target->team == this->airplane->team) {
			return;
		}
	}

	if (this->airplane->target != NULL) {
		if (this->airplane->target->type == AIRPLANE) {
			Airplane* target = (Airplane*) this->airplane->target;
			if (target->state == AIRPLANE_CRASHED || target->state == AIRPLANE_DESTROYED || target->team == this->airplane->team) {
				this->airplane->target == NULL;
			}
		}
	}

	if (World::entityACanSeeEntityB(this->airplane, entity) == true) {
		if (this->airplane->target == NULL) {
			this->airplane->target = entity;
		}
	} else {
		this->airplane->target = this->firstTarget;
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

	if (abs(toTarget.length()) < 0.0001) {
		return;
	}

	toTarget.normalize();

	float cos_angle = 1.0 - myFront.dot(toTarget);
	Vector3 axis = toTarget.cross(myFront);

	axis = modelInverse.rotateVector(axis);
	this->airplane->transform.rotate(cos_angle * deltaTime * 2.5, axis);

}


void AIController::patrol(float deltaTime) {

	if (this->airplane->target == NULL) {
		return;
	}

	if (World::distanceBetween(this->airplane, this->airplane->target) < 350) {
		this->airplane->target = this->airplane->path.selectNextWaypoint(this->airplane->target);
	}

	Matrix44 modelInverse = this->airplane->getGlobalMatrix();
	modelInverse.inverse();

	Vector3 myFront = this->airplane->getFront();

	Vector3 pos = this->airplane->getPosition();
	pos.y = 0;
	Vector3 targetPos = this->airplane->target->getPosition();
	targetPos.y = 0;
	Vector3 toTarget = (targetPos - pos);

	if (abs(toTarget.length()) < 0.0001) {
		return;
	}

	toTarget.normalize();

	float cos_angle = 1.0 - myFront.dot(toTarget);
	
	Vector3 axis = toTarget.cross(myFront);

	axis = modelInverse.rotateVector(axis);
	this->airplane->transform.rotate(cos_angle * deltaTime * 3.5, axis);

}