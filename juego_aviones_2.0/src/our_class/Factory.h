#ifndef FACTORY_H
#define FACTORY_H

#include "weapons/Bullet.h"
#include "World.h"
#include "weapons/Projectile.h"
#include "../framework.h"

class Airplane;
class EntityMesh;
class MachineGun;
class DropBomb;
class RocketLauncher;
class ShootGun;
class Bunker;

class Factory {

	public:

		static Airplane* buildAirplane(char team, const Vector3 pos, float speed);
		static Bunker* buildBunker(char team, const Vector3 pos);
		static EntityMesh* buildIsland(const Vector3 initialPos);
		static EntityMesh* buildWater(const Vector3 initialPos);
		static EntityMesh* buildSky(const Vector3 initialPos);
		static EntityMesh* buildSea(const Vector3 initialPos);
		static RocketLauncher* buildRocketLauncher(Airplane* owner);
		static MachineGun* buildMachineGun(Airplane* owner);
		static DropBomb* buildDropBomb(Airplane* owner);
		static ShootGun* buildShootGun(Airplane* owner);

	private:

		static Airplane* buildSpitfire(const Vector3 pos, float speed);
		static Airplane* buildBomber(const Vector3 pos, float speed);
		static Airplane* buildFighterPlaneP38(const Vector3 pos, float speed);
		static Airplane* buildWildcat(const Vector3 pos, float speed);

};

#endif // !FACTORY_H
