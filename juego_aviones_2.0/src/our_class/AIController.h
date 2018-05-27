#ifndef AICONTROLLER_H
#define AICONTROLLER_H
#pragma once

#include "AirplaneController.h"

enum Beheaviour {
	FLYING,
	CRHASED,
	SHOOT,
	FOLLOW,
	EVADE
};

class AIController : public AirplaneController{
	public:

		char state;

		AIController();
		~AIController();

		//virtual void render();
		virtual void update(float deltaTime);
		virtual void checkStateEnemy();
		void checkBehaviour();


};

#endif