#include "Factory.h"

Airplane* Factory::buildAirplane(const Vector3 initialPos, float speed) {
	Mesh* highMesh = Mesh::Load("data/box.ASE");
	//Mesh* lowMesh = Mesh::Load("data/spitfire/spitfire_low.ASE");
	Texture* texture = Texture::Load("data/spitfire/spitfire_color_spec.tga");
	Shader* shader = Shader::Load("data/shaders/texture.vs", "data/shaders/texture.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	static Airplane* airplane = new Airplane(speed, transform, highMesh, NULL, material);

	return airplane;
}

Bullet* Factory::buildBullet(const Vector3 pos, const Vector3 velocity, float power, float timeToLive, int author, std::string type) {
	static Bullet* bullet = new Bullet();
	bullet->lastPosition = pos;
	bullet->position = pos;
	bullet->velocity = velocity;
	bullet->power = power;
	bullet->timeToLive = timeToLive;
	bullet->type = type;
	bullet->author = author;
	return bullet;
}