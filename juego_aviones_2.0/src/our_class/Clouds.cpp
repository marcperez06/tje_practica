#include "Clouds.h"
#include "World.h"
#include "Material.h"
#include "../game.h"
#include <algorithm>

bool sortClouds(const Clouds::CloudProperties & cloudA, const Clouds::CloudProperties & cloudB) {
	if (cloudA.distance < cloudB.distance) {
		return true;
	}
	else {
		return false;
	}
}

Clouds::Clouds() : EntityMesh() {

	Texture* texture = Texture::Load("data/clouds/clouds.tga");
	Shader* shader = Shader::Load("data/shaders/clouds.vs", "data/shaders/clouds.fs");

	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1));

	this->material = material;

	this->clouds.resize(100);
	for (int i = 0; i < this->clouds.size(); i++) {
		CloudProperties & cloud = this->clouds[i];
		cloud.id = i;
		cloud.pos = Vector3();
		cloud.pos.random(Vector3(1000, 1000, 1000));
		cloud.pos.x *= 0.5;
		cloud.pos.z *= 0.5;
		cloud.pos = cloud.pos + Vector3(0, 300, 0);
		cloud.size = 50 + (random() * 100);
	}
}

Clouds::~Clouds() {}

void Clouds::render(Camera* camera) {
	Shader* shader = this->material->shader;
	Texture* texture = this->material->texture;

	Vector3 cameraPos = camera->eye;
	Vector3 top = camera->getLocalVector(Vector3(0, 1, 0));
	Vector3 right = camera->getLocalVector(Vector3(1, 0, 0));

	Mesh mesh;

	for (int i = 0; i < this->clouds.size(); i++) {
		this->clouds[i].distance = this->clouds[i].pos.distance(cameraPos);
	}
	
	std::sort(this->clouds.begin(), this->clouds.end(), sortClouds);

	for (int i = 0; i < this->clouds.size(); i++) {

		CloudProperties & cloud = this->clouds[i];
		
		Vector2 offset(0.5, 0.5);
		/*
		if (cloud.id % 4 == 1) {
			offset.x = 0.5;
		} else if (cloud.id % 4 == 2) {
			offset.y = 0.5;
		} else if (cloud.id % 4 == 3) {
			offset.x = 0.5;
			offset.y = 0.5;
		}
		*/

		Vector3 topRight = cloud.pos + (top + right) * cloud.size;
		Vector3 topLeft = cloud.pos + (top - right) * cloud.size;
		Vector3 bottomRight = cloud.pos + (-1 * top + right) * cloud.size;
		Vector3 bottomLeft = cloud.pos + (-1 * top - right) * cloud.size;

		mesh.vertices.push_back(topLeft);
		mesh.uvs.push_back(Vector2(0, 0) + offset);
		mesh.vertices.push_back(topRight);
		mesh.uvs.push_back(Vector2(0.5, 0) + offset);
		mesh.vertices.push_back(bottomRight);
		mesh.uvs.push_back(Vector2(0.5, 0.5) + offset);

		mesh.vertices.push_back(topLeft);
		mesh.uvs.push_back(Vector2(0, 0) + offset);
		mesh.vertices.push_back(bottomRight);
		mesh.uvs.push_back(Vector2(0.5, 0.5) + offset);
		mesh.vertices.push_back(bottomLeft);
		mesh.uvs.push_back(Vector2(0, 0.5) + offset);

		shader->enable();

		shader->setUniform("u_color", this->material->color);
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", this->material->texture);
		
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", Game::instance->time);

		glEnable(GL_BLEND);

		mesh.render(GL_TRIANGLES, shader);
		
		glDisable(GL_BLEND);

		shader->disable();

	}
}