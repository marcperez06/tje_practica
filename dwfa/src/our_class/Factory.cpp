#include "Factory.h"
#include "entities/Airplane.h"
#include "weapons/MachineGun.h"
#include "weapons/DropBomb.h"
#include "weapons/RocketLauncher.h"
#include "weapons/ShootGun.h"
#include "entities/Bunker.h"
#include "entities/ParticleSystem.h"

Airplane* Factory::buildAirplane(char team, const Vector3 pos, float speed) {

	Airplane* airplane = NULL;

	switch (team) {

		case TEAM_ALFA:
			airplane = Factory::buildSpitfire(pos, speed);
			break;
		case TEAM_DELTA:
			airplane = Factory::buildWildcat(pos, speed);
			break;
		/*case TEAM_BETA:
			airplane = Factory::buildFighterPlaneP38(pos, speed);
			break;
		case TEAM_GAMMA:
			airplane = Factory::buildBomber(pos, speed);
			break;*/

	}

	airplane->team = team;
	return airplane;

}

Bunker* Factory::buildBunker(char team, const Vector3 pos) {
	Mesh* highMesh = Mesh::Load("data/terrain_props/fort_base.ASE");
	//Mesh* lowMesh = Mesh::Load("data/terrain_props/spitfire_low.ASE");
	Texture* texture = Texture::Load("data/terrain_props/bunkers_and_forts.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/bunker.fs");

	Transform transform = Transform(pos, Quaternion());
	transform.matrixModel.scale(5, 5, 5);
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Bunker* bunker = new Bunker(transform, highMesh, material, team);

	return bunker;
}

Airplane* Factory::buildSpitfire(const Vector3 pos, float speed) {
	Mesh* highMesh = Mesh::Load("data/spitfire/spitfire.ASE");
	Mesh* lowMesh = Mesh::Load("data/spitfire/spitfire_low.ASE");
	Texture* texture = Texture::Load("data/spitfire/spitfire_color_spec.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");

	Transform transform = Transform(pos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Airplane* airplane = new Airplane(speed, transform, highMesh, lowMesh, material);
	airplane->weapons.push_back(Factory::buildMachineGun(airplane));
	airplane->currentWepon = 0;

	return airplane;
}

Airplane* Factory::buildBomber(const Vector3 pos, float speed) {
	Mesh* highMesh = Mesh::Load("data/bomber/bomber_axis.ASE");
	Texture* texture = Texture::Load("data/bomber/bomber_axis.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");

	Transform transform = Transform(pos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Airplane* airplane = new Airplane(speed, transform, highMesh, material);
	airplane->weapons.push_back(Factory::buildMachineGun(airplane));
	airplane->currentWepon = 0;

	return airplane;
}

Airplane* Factory::buildFighterPlaneP38(const Vector3 pos, float speed) {
	Mesh* highMesh = Mesh::Load("data/caza_p38/p38.ASE");
	Mesh* lowMesh = Mesh::Load("data/caza_p38/p38_low.ASE");
	Texture* texture = Texture::Load("data/caza_p38/p38.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");

	Transform transform = Transform(pos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Airplane* airplane = new Airplane(speed, transform, highMesh, lowMesh, material);
	airplane->weapons.push_back(Factory::buildMachineGun(airplane));
	airplane->currentWepon = 0;

	return airplane;
}

Airplane* Factory::buildWildcat(const Vector3 pos, float speed) {
	Mesh* highMesh = Mesh::Load("data/wildcat/wildcat.ASE");
	Mesh* lowMesh = Mesh::Load("data/wildcat/wildcat_low.ASE");
	Texture* texture = Texture::Load("data/wildcat/wildcat.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");

	Transform transform = Transform(pos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Airplane* airplane = new Airplane(speed, transform, highMesh, lowMesh, material);
	airplane->weapons.push_back(Factory::buildMachineGun(airplane));
	airplane->currentWepon = 0;

	return airplane;
}

EntityMesh* Factory::buildIsland(const Vector3 initialPos) {
	Mesh* highMesh = Mesh::Load("data/island/island.ASE");
	//Mesh* lowMesh = Mesh::Load("data/spitfire/island_low.ASE");
	Texture* texture = Texture::Load("data/island/island_color_luz.tga");
	Shader* shader = Shader::Load("data/shaders/texture.vs", "data/shaders/world.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	EntityMesh* island = new EntityMesh(transform, highMesh, material);

	//island->addChild(Factory::buildWater(island->getPosition()));

	return island;
}

EntityMesh* Factory::buildWater(const Vector3 initialPos) {
	Mesh* highMesh = Mesh::Load("data/island/water_deep.ASE");
	//Mesh* lowMesh = Mesh::Load("data/water/water_deep_low.ASE");
	Texture* texture = Texture::Load("data/island/water_deep.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/fog.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	EntityMesh* water = new EntityMesh(transform, highMesh, material);

	return water;
}

EntityMesh* Factory::buildSky(const Vector3 initialPos) {
	Mesh* highMesh = Mesh::Load("data/cielo/cielo.ASE");
	Texture* texture = Texture::Load("data/cielo/cielo.tga");
	Shader* shader = Shader::Load("data/shaders/texture.vs", "data/shaders/texture.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);
	
	EntityMesh* sky = new EntityMesh(transform, highMesh, material);

	return sky;
}

EntityMesh* Factory::buildSea(const Vector3 initialPos) {
	Mesh* highMesh = new Mesh();
	highMesh->createSubdividedPlane(130000, 50, true);
	//Texture* texture = Texture::Load("data/water/water_normalmap.tga");
	//Texture* texture = Texture::Load("data/cielo/cielo.tga");
	Texture* texture = Texture::Load("data/water/agua.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/water.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);
	material->extraTexture = Texture::Load("data/cielo/cielo.tga");
	material->normalTexture = Texture::Load("data/water/water_normalmap.tga");
	//material->extraTexture = Texture::Load("data/water/agua.tga");

	EntityMesh* sea = new EntityMesh(transform, highMesh, material);

	//sea->transform.matrixModel.scale(10000, 10000, 10000);

	return sea;
}

RocketLauncher* Factory::buildRocketLauncher(Airplane* owner) {
	RocketLauncher* rocketLauncher = new RocketLauncher(owner, ROCKET_LAUNCHER);
	rocketLauncher->damage = 400;
	rocketLauncher->bulletSpeed = 300;
	rocketLauncher->cooldown = 0;
	rocketLauncher->fireRate = 5;
	return rocketLauncher;
}

MachineGun* Factory::buildMachineGun(Airplane* owner) {
	MachineGun* machineGun = new MachineGun(owner, MACHINE_GUN);
	machineGun->damage = 60;
	machineGun->bulletSpeed = 2.5;
	machineGun->cooldown = 0;
	machineGun->fireRate = 10;
	return machineGun;
}

DropBomb* Factory::buildDropBomb(Airplane* owner) {
	DropBomb* weapon = new DropBomb(owner, DROP_BOMB);
	weapon->damage = 600;
	weapon->bulletSpeed = 1;
	weapon->cooldown = 0;
	weapon->fireRate = 5;
	return weapon;
}

ShootGun* Factory::buildShootGun(Airplane* owner) {
	ShootGun* shootGun = new ShootGun(owner, SHOOT_GUN);
	shootGun->damage = 60;
	shootGun->bulletSpeed = 2.5;
	shootGun->cooldown = 0;
	shootGun->fireRate = 10;
	return shootGun;
}