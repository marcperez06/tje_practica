#include "AirplaneController.h"
#include "Airplane.h"

AirplaneController::AirplaneController() {}
AirplaneController::~AirplaneController() {}

bool AirplaneController::canSeeEntity(Entity* entity) {
	float angleOfview = 45 / 2.0;
	bool canSee = false;
	assert(entity);

	Matrix44 modelInverse = this->airplane->getGlobalMatrix();
	modelInverse.inverse();

	Vector3 pos = this->airplane->getGlobalPosition();
	Vector3 entityPos = entity->getGlobalPosition();

	Vector3 toEntity = (entityPos - pos);

	if (abs(toEntity.length()) < 0.0001) {
		canSee = false;
	}

	toEntity.normalize();

	Vector3 front = this->airplane->getGlobalMatrix().rotateVector(Vector3(0, 0, -1));

	if (abs(front.length()) < 0.0001) {
		canSee = false;
	}

	front.normalize();

	float frontDotToEntity = front.dot(toEntity);
	if (abs(frontDotToEntity) >= 1) {
		canSee = false;
	}

	float angle = acos(frontDotToEntity);

	if (abs(angle) <= angleOfview) {
		canSee = true;
	} else {
		canSee = false;
	}

	return canSee;
}

bool AirplaneController::haveAnyEntityNear(Entity* entity) {
	bool haveAnyEntityNear = false;

	Vector3 pos = this->airplane->getGlobalPosition();


	return haveAnyEntityNear;
}