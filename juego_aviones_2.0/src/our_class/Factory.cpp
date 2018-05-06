#include "Factory.h"

Airplane* Factory::buildAirplane(const Vector3 initialPos, float speed) {
	Mesh* highMesh = Mesh::Load("data/spitfire/spitfire.ASE");
	Mesh* lowMesh = Mesh::Load("data/spitfire/spitfire_low.ASE");
	Texture* texture = Texture::Load("data/spitfire/spitfire_color_spec.tga");
	Shader* shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Airplane* airplane = new Airplane(speed, transform, highMesh, NULL, material);

	return airplane;
}

Bullet* Factory::buildBullet(const Vector3 pos, const Vector3 velocity, float power, float timeToLive, int author, std::string type) {
	Bullet* bullet = new Bullet();
	bullet->lastPosition = pos;
	bullet->position = pos;
	bullet->velocity = velocity;
	bullet->power = power;
	bullet->timeToLive = timeToLive;
	bullet->type = type;
	bullet->author = author;
	return bullet;
}

EntityMesh* Factory::buildIsland(const Vector3 initialPos) {
	Mesh* highMesh = Mesh::Load("data/island/island.ASE");
	//Mesh* lowMesh = Mesh::Load("data/spitfire/island_low.ASE");
	Texture* texture = Texture::Load("data/island/island_color_luz.tga");
	Shader* shader = Shader::Load("data/shaders/texture.vs", "data/shaders/texture.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	EntityMesh* island = new EntityMesh(transform, highMesh, material);

	island->addChild(Factory::buildWater(island->getPosition()));

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