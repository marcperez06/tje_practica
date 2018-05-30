#ifndef PATH
#define PATH

#include "../framework.h"

class Entity;

class Path{
public:

	std::vector<Entity*> positions;

	Path();
	~Path();

	/*
	virtual void render();
	virtual void update(float deltaTime);
	virtual void checkStateEnemy();
	char checkBehaviour();

	void selectTarget(Airplane* entity);
	void followTarget();*/


};

#endif