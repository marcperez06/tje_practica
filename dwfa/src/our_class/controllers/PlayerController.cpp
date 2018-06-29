#include "PlayerController.h"
#include "../entities/Airplane.h"
#include "../../input.h"
#include "../weapons/Weapon.h"


PlayerController::PlayerController() : AirplaneController() {}
PlayerController::~PlayerController() {}

void PlayerController::update(float deltaTime) {

	this->rotateAirplane(deltaTime);

	this->turbo(deltaTime);

	selectWeapon();

	if (Input::isKeyPressed(SDL_SCANCODE_SPACE) == true || abs(Input::gamepads[0].axis[TRIGGERS]) > 0.2) {
		this->airplane->shoot();
	}

}

void PlayerController::rotateAirplane(float deltaTime) {

	float deltaMove = this->airplane->speed * deltaTime * 0.03;
	this->rotateRollDirection(deltaMove * 0.3);
	this->rotatePitchDirection(deltaMove * 0.1);
	this->rotateYawDirection(deltaMove * 0.1);
}

void PlayerController::rotateRollDirection(float deltaMove) {
	// roll right
	if (Input::isKeyPressed(SDL_SCANCODE_Q) == true || Input::isButtonPressed(RB_BUTTON) == true) {
		this->airplane->rotateRollDirection(-1 * deltaMove);
	}

	// roll left
	if (Input::isKeyPressed(SDL_SCANCODE_E) == true || Input::isButtonPressed(LB_BUTTON) == true) {
		this->airplane->rotateRollDirection(deltaMove);
	}
}

void PlayerController::rotatePitchDirection(float deltaMove) {
	// down
	if (Input::isKeyPressed(SDL_SCANCODE_S) == true || Input::isButtonPressed(LEFT_ANALOG_Y) == true) {
		this->airplane->rotatePitchDirection(deltaMove);
	}

	// up
	if (Input::isKeyPressed(SDL_SCANCODE_W) == true || Input::isButtonPressed(LEFT_ANALOG_Y) == true) {
		this->airplane->rotatePitchDirection(-1 * deltaMove);
	}
}

void PlayerController::rotateYawDirection(float deltaMove) {
	// yaw right
	if (Input::isKeyPressed(SDL_SCANCODE_D) == true || Input::isButtonPressed(LEFT_ANALOG_X) == true) {
		this->airplane->rotateYawDirection(deltaMove);
	}

	// yaw left
	if (Input::isKeyPressed(SDL_SCANCODE_A) == true || Input::isButtonPressed(LEFT_ANALOG_X) == true) {
		this->airplane->rotateYawDirection(-1 * deltaMove);
	}
}

void PlayerController::turbo(float deltaTime) {
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) == true) {
		this->airplane->turbo(deltaTime);
	}

	if (Input::isKeyPressed(SDL_SCANCODE_TAB) == true) {
		this->airplane->turbo(-1 * deltaTime);
	}
}

void PlayerController::selectWeapon() {
	
	if (Input::wasKeyPressed(SDL_SCANCODE_0) == true || Input::wasButtonPressed(A_BUTTON) == true) {
		this->airplane->selectWeapon(MACHINE_GUN);
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_9) == true || Input::wasButtonPressed(B_BUTTON) == true) {
		this->airplane->selectWeapon(DROP_BOMB);
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_8) == true || Input::wasButtonPressed(Y_BUTTON) == true) {
		this->airplane->selectWeapon(ROCKET_LAUNCHER);
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_7) == true || Input::wasButtonPressed(X_BUTTON) == true) {
		this->airplane->selectWeapon(SHOOT_GUN);
	}

}