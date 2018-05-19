#ifndef FACTORY_H
#define FACTORY_H

#include "Bullet.h"
#include "../framework.h"

class Airplane;
class EntityMesh;
class MachineGun;
class DropBomb;

class Factory {

	public:

		static Airplane* buildAirplane(const Vector3 initialPos, float speed);
		static Bullet buildBullet(const Vector3 pos, const Vector3 velocity, float timeToLive, std::string type, Airplane* owner);
		static EntityMesh* buildIsland(const Vector3 initialPos);
		static EntityMesh* buildWater(const Vector3 initialPos);
		static EntityMesh* buildSky(const Vector3 initialPos);
		static EntityMesh* buildMisil(const Vector3 initialPos);
		static MachineGun* buildMachineGun(Airplane* owner);
		static DropBomb* buildDropBomb(Airplane* owner);

};

#endif // !FACTORY_H
