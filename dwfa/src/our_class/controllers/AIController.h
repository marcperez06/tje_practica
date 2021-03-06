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
		Entity* firstTarget;
		 
		AIController();
		AIController(Airplane* ariplane);
		~AIController();

		virtual void render();
		virtual void update(float deltaTime);

		char checkBehaviour();

		void selectTarget(Entity* entity);
		void followTarget(float deltaTime);
		void patrol(float deltaTime);

};

#endif