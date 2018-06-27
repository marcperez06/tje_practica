#include "ProjectileManager.h"
#include "../World.h"
#include "../../extra/coldet/coldet.h"
#include "../entities/EntityCollider.h"
#include "CollisionHandler.h"

ProjectileManager* ProjectileManager::instance = NULL;

ProjectileManager::ProjectileManager() {
	memset(&this->bombs, 0, sizeof(this->bombs));
	memset(&this->misils, 0, sizeof(this->misils));
	this->instance = this;
}

ProjectileManager::~ProjectileManager() {
	for (int i = 0; i < maxBombs; i++) {
		this->bombs[i].timeToLive = 0;
	}

	for (int i = 0; i < maxMisils; i++) {
		this->misils[i].timeToLive = 0;
	}
}

void ProjectileManager::createProjectile(const Matrix44 transform, Vector3 velocity, std::string type, Airplane* owner, int damage) {

	Projectile projectile;

	if (type.compare("bomb") == 0) {
		projectile.setProperties(transform, velocity, 300, type, owner, damage);

		for (int i = 0; i < maxBombs; i++) {
			Projectile& auxBomb = this->bombs[i];
			if (auxBomb.timeToLive > 0) {
				continue;
			}

			auxBomb = projectile;
			break;
		}

	} else {
		projectile.setProperties(transform, velocity, 30, type, owner, damage);

		for (int i = 0; i < maxMisils; i++) {
			Projectile& auxMisil = this->misils[i];
			if (auxMisil.timeToLive > 0) {
				continue;
			}

			auxMisil = projectile;
			break;
		}

	}

	

}

void ProjectileManager::render() {

	this->renderBombs();
	this->renderMisils();

}

void ProjectileManager::renderBombs() {
	std::vector<Matrix44> bombsTransform;
	Mesh* mesh = Mesh::Load("data/weapons/bomb.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");
	//Shader* shader = this->meshMisil->material->shader;
	Texture* texture = Texture::Load("data/weapons/bomb.tga");
	Camera* camera = World::instance->currentCamera;

	if (mesh == NULL || shader == NULL) {
		return;
	}

	for (int i = 0; i < maxBombs; i++) {
		Projectile& bomb = this->bombs[i];

		if (bomb.timeToLive > 0) {
			bombsTransform.push_back(bomb.transform);
		}
	}

	if (bombsTransform.size() > 0) {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", 1);
		shader->setUniform("u_texture", texture);

		mesh->renderInstanced(GL_TRIANGLES, shader, &bombsTransform[0], bombsTransform.size());

		shader->disable();
	}
}

void ProjectileManager::renderMisils() {
	std::vector<Matrix44> misilsTransform;
	Mesh* mesh = Mesh::Load("data/weapons/torpedo.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");
	//Shader* shader = this->meshMisil->material->shader;
	Texture* texture = Texture::Load("data/weapons/torpedo.tga");
	Camera* camera = World::instance->currentCamera;

	if (mesh == NULL || shader == NULL) {
		return;
	}

	for (int i = 0; i < maxMisils; i++) {
		Projectile& misil = this->misils[i];

		if (misil.timeToLive > 0) {
			misilsTransform.push_back(misil.transform);

			std::cout << "Projectile X " << misil.position.x << std::endl;
			std::cout << "Projectile XX " << misil.transform.getTranslation().x << std::endl;

			std::cout << "Projectile Y " << misil.position.y << std::endl;
			std::cout << "Projectile YY " << misil.transform.getTranslation().y << std::endl;

			std::cout << "Projectile Z " << misil.position.z << std::endl;
			std::cout << "Projectile ZZ " << misil.transform.getTranslation().z << std::endl;

		}
	}

	if (misilsTransform.size() > 0) {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", 1);
		shader->setUniform("u_texture", texture);

		mesh->renderInstanced(GL_TRIANGLES, shader, &misilsTransform[0], misilsTransform.size());

		shader->disable();
	}
}

void ProjectileManager::update(float deltaTime) {

	this->updateBombs(deltaTime);
	this->updateMisils(deltaTime);

}

void ProjectileManager::updateBombs(float deltaTime) {
	for (int i = 0; i < maxBombs; i++) {
		Projectile& bomb = this->bombs[i];

		if (bomb.timeToLive < 0) {
			continue;
		}

		Vector3 newPos = bomb.velocity * deltaTime;
		bomb.lastPosition = bomb.transform.getTranslation();
		bomb.transform.translate(newPos.x, newPos.y, newPos.z);
		bomb.position = bomb.transform.getTranslation();
		bomb.velocity = bomb.velocity + Vector3(0, -9.8, 0) * deltaTime; // aplicar gravedad
		bomb.timeToLive -= deltaTime;

	}

	CollisionHandler::bulletsCollisionAgainstStaticEntities(this->bombs, maxBombs);
	CollisionHandler::bulletsCollisionAgainstDynamicEntities(this->bombs, maxBombs);
}

void ProjectileManager::updateMisils(float deltaTime) {
	for (int i = 0; i < maxMisils; i++) {
		Projectile& misil = this->misils[i];

		if (misil.timeToLive <= 0) {
			continue;
		}

		Vector3 newPos = misil.velocity * deltaTime;
		misil.lastPosition = misil.transform.getTranslation();
		misil.transform.translate(newPos.x, newPos.y, newPos.z);
		misil.position = misil.transform.getTranslation();
		misil.velocity = misil.velocity + Vector3(0, -2, 0) * deltaTime; // aplicar gravedad
		misil.velocity = misil.velocity * 0.99999995;
		misil.timeToLive -= deltaTime;

	}

	CollisionHandler::bulletsCollisionAgainstStaticEntities(this->misils, maxMisils);
	CollisionHandler::bulletsCollisionAgainstDynamicEntities(this->misils, maxMisils);
}