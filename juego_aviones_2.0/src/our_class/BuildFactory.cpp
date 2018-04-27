#include "BuildFactory.h"
#include "Airplane.h"
#include "../mesh.h"
#include "../texture.h"
#include "../shader.h"

Airplane* BuildFactory::buildPlayer(Vector3 const initialPos) {
	float speed = 50;
	Mesh* highMesh = Mesh::Load("data/spitfire/spitfire.ASE");
	Mesh* lowMesh = Mesh::Load("data/spitfire/spitfire_low.ASE");
	Texture* texture = Texture::Load("data/spitfire/spitfire_color_spec.tga");
	Shader* shader = Shader::Load("data/shaders/texture.vs", "data/shaders/texture.fs");

	Transform transform = Transform(initialPos, Quaternion());
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1), false, true, false);

	Airplane* player = new Airplane(speed, transform, highMesh, lowMesh, material);

	return player;
}