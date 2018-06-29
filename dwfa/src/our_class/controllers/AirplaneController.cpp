#include "AirplaneController.h"
#include "../entities/Airplane.h"

AirplaneController::AirplaneController() {}

AirplaneController::AirplaneController(Airplane* airplane) {
	this->airplane = airplane;
}

AirplaneController::~AirplaneController() {}

void AirplaneController::render() {}
void AirplaneController::update(float deltaTime) {}
