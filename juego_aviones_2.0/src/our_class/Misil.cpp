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
		Matrix44 ownerMatrix = this->owner->getGlobalMatrix();
		Matrix44 misilTransform = this->owner->getGlobalMatrix();
		misilTransform.translate(0, -2, 0);

		Vector3 velocity = Vector3(0, 0, -1);
		velocity = velocity * this->bulletSpeed;

		Projectile misil = Factory::buildProjectile(misilTransform, velocity, 30, this->type, this->owner, this->damage);

		for (int i = 0; i < maxMisil; i++) {
			Projectile& auxMisil = this->misils[i];
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

	std::vector<Matrix44> misilTransform;
	Mesh* mesh = Mesh::Load("data/weapons/torpedo.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");
	//Shader* shader = this->meshMisil->material->shader;
	Texture* texture = Texture::Load("data/weapons/torpedo.tga");
	Camera* camera = World::instance->currentCamera;

	if (mesh == NULL || shader == NULL) {
		return;
	}

	for (int i = 0; i < maxMisil; i++) {
		Projectile& misil = this->misils[i];

		if (misil.timeToLive > 0) {
			misilTransform.push_back(misil.transform);
		}
	}

	if (misilTransform.size() > 0) {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", 1);
		shader->setUniform("u_texture", texture);

		mesh->renderInstanced(GL_TRIANGLES, shader, &misilTransform[0], misilTransform.size());

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
		Projectile& misil = this->misils[i];

		if (misil.timeToLive <= 0) {
			continue;
		}

		
		Vector3 newPos = misil.velocity * deltaTime;
		misil.lastPosition = misil.transform.getTranslation();
		misil.transform.translate(newPos.x, newPos.y, newPos.z);
		misil.position = misil.transform.getTranslation();
		//misil.velocity = misil.velocity + Vector3(0, -2, 0) * deltaTime; // aplicar gravedad
		//misil.velocity = misil.velocity * 0.99999995;
		misil.timeToLive -= deltaTime;

	}

	//CollisionHandler::bulletsCollisionAgainstStaticEntities(this->misils, maxMisil);

}