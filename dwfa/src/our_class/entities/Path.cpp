#include "Path.h"
#include "Entity.h"

Path::Path() {
	
}

Path::~Path() {}


void Path::createCircle(Vector3 center, int radius, int heigth) {

	Vector3 up = Vector3::Vector3(center.x, heigth, center.z+radius);
	createWayPoint(up);
	Vector3 upright = Vector3::Vector3(center.x + radius/sqrt(2), heigth, center.z + radius/sqrt(2));
	createWayPoint(upright);
	Vector3 rigth = Vector3::Vector3(center.x + radius, heigth, center.z);
	createWayPoint(rigth);
	Vector3 downrigth = Vector3::Vector3(center.x + radius/sqrt(2), heigth, center.z - radius/sqrt(2));
	createWayPoint(downrigth);
	Vector3 down = Vector3::Vector3(center.x, heigth, center.z - radius);
	createWayPoint(down);
	Vector3 downleft = Vector3::Vector3(center.x - radius/sqrt(2), heigth, center.z - radius / sqrt(2));
	createWayPoint(downleft);
	Vector3 left = Vector3::Vector3(center.x - radius, heigth, center.z);
	createWayPoint(left);
	Vector3 upleft = Vector3::Vector3(center.x - radius / sqrt(2), heigth, center.z + radius / sqrt(2));
	createWayPoint(upleft);

}

void Path::createWayPoint(Vector3 pos) {
	Entity* wayPoint = new Entity(pos);
	this->wayPoints.push_back(wayPoint);
	//delete wayPoint;
}

Entity* Path::selectNextWaypoint(Entity* wp) {
	for (int i = 0; i < this->wayPoints.size(); i++)
	{
		if (this->wayPoints[i] == wp) {
			if (this->wayPoints.size() - 1 == i) {
				return this->wayPoints[0];
			}
			else {
				return this->wayPoints[i + 1];
			}
			
		}
	}
}