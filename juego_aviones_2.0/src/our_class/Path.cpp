#include "Path.h"

Path::Path() {
	
}

Path::~Path() {}


void Path::createCircle(Vector3 center, int radius, int points) {

	Entity* wayPoint;
	for (int i = 0; i < points; i++)
	{
		//Vector3 wayPos = new Vector3(center.x+radius, 0, center.z);
	}
	this->wayPoints.push_back(wayPoint);

}





/*
Crear 4 equips de 10 avions cadascun
Cada equip tindra una base (EntityCollider)
Els avions de cada equip tenen tres modos: Atacar, Patrullar i DefensarBase
Els avions solament comennšen amb la machinGun
Powerups per l'aire (solament per el player) que contindran: 5 misils, 5 bombes, machinGunFree, shootGun
Objectiu de cada equip tirar les bases rivals.
Si s'elimina la base rival els avions d'aquell equip passen a ser del teu equip.

Tema GUI:
	- Indicador per la vida del player (NUMERO)
	- Quadrats per les armes que te el player
	- Identificadors de base rival i de avions rivals llunyans
*/