#include "Misil.h"
#include "Airplane.h"
#include "Factory.h"
#include "../Camera.h"
#include "World.h"
#include "CollisionHandler.h"

Misil::Misil(Airplane* owner, std::string type) : Weapon(owner, type) {
	memset(&this->misils, 0, sizeof(misils));

}

void Misil::shoot() {
	if (this->cooldown < 0) {

		Matrix44 modelMatrix = this->owner->getGlobalMatrix();
		Vector3 pos = modelMatrix * Vector3(0, -2, 0);
		Vector3 velocity = modelMatrix.rotateVector(Vector3(0, 0, -1));
		velocity = velocity * this->bulletSpeed;

		Bullet misil = Factory::buildBullet(pos, velocity, 30, this->type, this->owner, this->damage);

		for (int i = 0; i < maxMisil; i++) {
			Bullet& auxMisil = this->misils[i];
			if (auxMisil.timeToLive > 0) {
				continue;
			}

			auxMisil = misil;
			break;
		}

		this->cooldown = 5;
	}
}

void Misil::render() {

	std::vector<Matrix44> misilPos;
	Mesh* mesh = this->meshMisil->highMesh;
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");
	//Shader* shader = this->meshMisil->material->shader;
	Texture* texture = this->meshMisil->material->texture;
	Camera* camera = World::instance->currentCamera;
	Matrix44 identity;

	if (mesh == NULL || shader == NULL) {
		return;
	}

	for (int i = 0; i < maxMisil; i++) {
		Bullet& misil = this->misils[i];
		if (misil.timeToLive > 0) {

			Matrix44 modelMisil;
			modelMisil.translate(misil.position.x, misil.position.y, misil.position.z);
			modelMisil.rotate(1.57, Vector3(0, 1, 0));

			misilPos.push_back(modelMisil);
		}
	}

	if (misilPos.size() > 0) {
		shader->enable();

		shader->setUniform("u_color", this->meshMisil->material->color);
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", 1);
		shader->setUniform("u_texture", texture);

		mesh->renderInstanced(GL_TRIANGLES, shader, &misilPos[0], misilPos.size());

		shader->disable();
	}

	/*
	Mesh mesh;
	for (int i = 0; i < maxMisil; i++) {
		Bullet& misil = this->misils[i];
		if (misil.timeToLive > 0) {
			mesh.vertices.push_back(misil.position);
			mesh.colors.push_back(Vector4(1, 0, 0, 1));
		}
	}

	if (mesh.vertices.size() > 0) {
		glPointSize(50);
		mesh.renderFixedPipeline(GL_POINTS);
	}
	*/

}

void Misil::update(float deltaTime) {
	Weapon::update(deltaTime);

	for (int i = 0; i < maxMisil; i++) {
		Bullet& misil = this->misils[i];

		if (misil.timeToLive <= 0) {
			continue;
		}

		misil.lastPosition = misil.position;
		misil.position = misil.position + misil.velocity * deltaTime;
		misil.velocity = misil.velocity + Vector3(0, -2, 0) * deltaTime; // aplicar gravedad
		misil.velocity = misil.velocity * 0.99999995;
		misil.timeToLive -= deltaTime;

	}

	CollisionHandler::bulletsCollisionAgainstStaticEntities(this->misils, maxMisil);

}