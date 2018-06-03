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





/*
Crear 4 equips de 10 avions cadascun
Cada equip tindra una base (EntityCollider)
Els avions de cada equip tenen tres modos: Atacar, Patrullar i DefensarBase
Els avions solament comennçen amb la machinGun
Powerups per l'aire (solament per el player) que contindran: 5 misils, 5 bombes, machinGunFree, shootGun
Objectiu de cada equip tirar les bases rivals.
Si s'elimina la base rival els avions d'aquell equip passen a ser del teu equip.

Tema GUI:
	- Indicador per la vida del player (NUMERO)
	- Quadrats per les armes que te el player
	- Identificadors de base rival i de avions rivals llunyans
*/