#ifndef AICONTROLLER_H
#define AICONTROLLER_H
#pragma once

#include "AirplaneController.h"

enum Beheaviour {
	FLYING,
	CRHASED,
	SHOOT,
	FOLLOW,
	EVADE,
	ESCAPE,
	PATROL
};

class Entity;

class AIController : public AirplaneController{
	public:

		char state;

		AIController();
		~AIController();

		virtual void render();
		virtual void update(float deltaTime);
		virtual void checkStateEnemy();
		char checkBehaviour();

		void selectTarget(Entity* entity);
		void followTarget(float deltaTime);


};

#endif