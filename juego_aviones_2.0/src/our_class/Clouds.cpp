#include "Clouds.h"
#include "World.h"
#include "../game.h"

Clouds::Clouds() : EntityMesh() {

	Texture* texture = Texture::Load("data/clouds/cloud.tga");
	Shader* shader = Shader::Load("data/shaders/clouds.vs", "data/shaders/clouds.fs");

	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1));

	this->clouds.resize(100);
	for (int i = 0; i < this->clouds.size(); i++) {
		CloudProperties & cloud = this->clouds[i];
		cloud.id = i;
		cloud.pos = Vector3().random(3000);
		cloud.pos.x *= 0.5;
		cloud.pos.z *= 0.5;
		cloud.pos = cloud.pos + Vector3(0, 1000, 0);
		cloud.size = 30 + (random() * 100);
	}
}

void Clouds::render() {
	Camera* camera = World::instance->currentCamera;
	Shader* shader = this->material->shader;
	Texture* texture = this->material->texture;

	Vector3 top = camera->getLocalVector(Vector3(0, 1, 0));
	Vector3 right = camera->getLocalVector(Vector3(1, 0, 0));
	Mesh mesh;

	for (int i = 0; i < clouds.size(); i++) {

		CloudProperties & cloud = this->clouds[i];
		
		Vector2 offset(0, 0);
		if (cloud.id % 4 == 1) {
			offset.x = 0.5;
		} else if (cloud.id % 4 == 2) {
			offset.y = 0.5;
		} else if (cloud.id % 4 == 3) {
			offset.x = 0.5;
			offset.y = 0.5;
		}

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