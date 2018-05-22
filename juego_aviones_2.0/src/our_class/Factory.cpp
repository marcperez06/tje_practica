#include "Factory.h"
#include "Airplane.h"
#include "MachineGun.h"
#include "DropBomb.h"
#include "Misil.h"

Airplane* Factory::buildAirplane(const Vector3 initialPos, float speed) {
	Mesh* highMesh = Mesh::Load("data/spitfire/spitfire.ASE");
	Mesh* lowMesh = Mesh::Load("data/spitfire/spitfire_low.ASE");
	Texture* texture = Texture::Load("data/spitfire/spitfire_color_spec.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Airplane* airplane = new Airplane(speed, transform, highMesh, lowMesh, material);

	return airplane;
}

Bullet Factory::buildBullet(const Vector3 pos, const Vector3 velocity, float timeToLive, std::string type, Airplane* owner) {
	Bullet bullet;
	bullet.lastPosition = pos;
	bullet.position = pos;
	bullet.velocity = velocity;
	bullet.timeToLive = timeToLive;
	bullet.type = type;
	bullet.owner = owner;
	return bullet;
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


Misil* Factory::buildMisil(Airplane* owner) {
	Misil* weapon = new Misil(owner, "misil");
	weapon->damage = 400;
	weapon->bulletSpeed = 230;
	weapon->cooldown = 5;
	weapon->fireRate = 5;
	
	Mesh* highMesh = Mesh::Load("data/weapons/torpedo.ASE");
	Texture* texture = Texture::Load("data/weapons/torpedo.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");
	Transform transform = Transform(owner->getPosition(), Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);
	
	weapon->meshMisil = new EntityMesh(transform, highMesh, material);

	return weapon;
}

MachineGun* Factory::buildMachineGun(Airplane* owner) {
	MachineGun* machineGun = new MachineGun(owner, "bullet");
	machineGun->damage = 60;
	machineGun->bulletSpeed = 2.5;
	machineGun->cooldown = 0.50;
	machineGun->fireRate = 10;
	return machineGun;
}

DropBomb* Factory::buildDropBomb(Airplane* owner) {
	DropBomb* weapon = new DropBomb(owner, "bomb");
	weapon->damage = 600;
	weapon->bulletSpeed = 1;
	weapon->cooldown = 10;
	weapon->fireRate = 5;
	return weapon;
}