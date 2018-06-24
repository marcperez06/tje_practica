#ifndef PATH
#define PATH

#include "../../framework.h"

class Entity;

class Path{
public:

	std::vector<Entity*> wayPoints;

	Path();
	~Path();

	void createCircle(Vector3 center, int radius, int heigth);
	void createWayPoint(Vector3 pos);

};

#endif