#ifndef AICONTROLLER_H
#define AICONTROLLER_H

enum {
	AIRPLANE_FLYING,
	AIRPLANE_CRHASED,
	AIRPLANE_SHOOT,
	AIRPLANE_FOLLOW,
};

class AIController {
public:

	char state;

	AIController::AIController();
	AIController::~AIController();

	virtual void AIController::render();
	virtual void AIController::update(float deltaTime);
	virtual void AIController::checkStateEnemy();

};

#endif