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
				//this->airplane->target = 
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

	if (World::distanceBetween(this->airplane, this->airplane->target) < 150) {
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

	if (this->airplane->target != NULL) {
		return;
	}

	Airplane* target = NULL;

	if (dynamic_cast<Airplane*>(this->airplane->target)) {
		target = (Airplane*) this->airplane->target;
		if (target->state != AIRPLANE_CRASHED && target->state != AIRPLANE_DESTROYED) {
			return;
		}
	}

	Airplane* airplaneEntity = NULL;

	if (this->airplane->target->type == AIRPLANE) {
		Airplane* airplaneEntity = (Airplane*) this->airplane->target;
		if (airplaneEntity->team == this->airplane->team) {
			return;
		}
	}

	if (World::entityACanSeeEntityB(this->airplane, entity) == true) {
		this->airplane->target = entity;
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
	Vector3 direction = (this->airplane->target->getPosition() - this->airplane->getPosition());
	Vector3 entityFront = this->airplane->getGlobalMatrix().frontVector();
	Vector3 entityRight = this->airplane->getGlobalMatrix().rightVector();

	direction.y = 0.0;
	direction.normalize();
	entityFront.y = 0.0;
	direction.normalize();
	entityRight.y = 0.0;
	direction.normalize();

	//Necessitem que no tingui en compte la diferencia d'al�ada, ja que nom�s rota en un pla
	//Per aix� passem de les Y, sin� rota encara que estigui alineada amb el target (per� en diferent al�ada)
	double dot = (direction.x * entityFront.x) + (direction.z * entityFront.z);
	double dot_frontUp = (direction.x * entityRight.x) + (direction.z * entityRight.z);
	float angle = acos(dot);

	//Es torreta aka enfocar en el pla xz rota sobre el seu eix Y
	if (angle > 0.05)
	{
		if (dot_frontUp > 0.0) {
			this->airplane->transform.rotate(float(-deltaTime * angle), this->airplane->getGlobalMatrix().topVector());
		}
		else if (dot_frontUp < 0.0) {
			this->airplane->transform.rotate(float(deltaTime * angle), this->airplane->getGlobalMatrix().topVector());
		}
	}
}

/*
void AIController::followTarget(float deltaTime) {

	if (this->airplane->target == NULL) {
		return;
	}

	Airplane* airplaneEntity = NULL;

	if (this->airplane->target->type == AIRPLANE) {
		Airplane* airplaneEntity = (Airplane*) this->airplane->target;
		if (airplaneEntity->health <= 0) {
			return;
		}
	}

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

	if (abs(toTarget.length()) < 0.0001) {
		return;
	}

	toTarget.normalize();

	Vector3 front = this->airplane->getGlobalMatrix().rotateVector(Vector3(0, 0, -1));

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
		//this->airplane->shoot();
		return;
	}

	Vector3 axis = front.cross(toTarget);
	axis = modelInverse.rotateVector(axis);
	axis.normalize();

	this->airplane->transform.rotate(angle, axis);

	if (this->airplane->target->getGlobalPosition().x == this->airplane->getGlobalPosition().x) {
		if (this->airplane->target->getGlobalPosition().y == this->airplane->getGlobalPosition().y) {
			if (this->airplane->target->getGlobalPosition().z == this->airplane->getGlobalPosition().z) {
				std::cout << " ------------------- HE LLEGADO -------------" << std::endl;
			}
		}
	}

}
*/