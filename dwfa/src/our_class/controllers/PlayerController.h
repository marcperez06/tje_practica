#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "AirplaneController.h"
#pragma once

class PlayerController : public AirplaneController {

	public:

		PlayerController();
		~PlayerController();

		virtual void update(float deltaTime);

	private:

		void rotateAirplane(float deltaTime);
		void rotateRollDirection(float deltaMove);
		void rotatePitchDirection(float deltaMove);
		void rotateYawDirection(float deltaMove);
		void turbo(float deltaTime);
		void selectWeapon();

};

#endif