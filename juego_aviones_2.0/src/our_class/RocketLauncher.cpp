#include "RocketLauncher.h"
#include "Airplane.h"
#include "Factory.h"
#include "../Camera.h"
#include "World.h"
#include "CollisionHandler.h"

RocketLauncher::RocketLauncher(Airplane* owner, char type) : Weapon(owner, type) {
	memset(&this->misils, 0, sizeof(misils));
}

void RocketLauncher::shoot() {
	if (this->cooldown < 0 && this->ammounition > 0) {

		Matrix44 misilTransform = this->owner->getGlobalMatrix();
		misilTransform.translate(0, -2, 0);
		misilTransform.rotate(180 * DEG2RAD, Vector3(0, 1, 0));

		Vector3 velocity = Vector3(0, 0, 1);

		velocity = velocity * this->bulletSpeed;

		Projectile misil;
		misil.setProperties(misilTransform, velocity, 30, "rocketLauncher", this->owner, this->damage);

		for (int i = 0; i < maxMisil; i++) {
			Projectile& auxMisil = this->misils[i];
			if (auxMisil.timeToLive > 0) {
				continue;
			}

			auxMisil = misil;
			break;
		}

		this->cooldown = 5;
		this->ammounition -= 1;
	}
}

void RocketLauncher::render() {

	std::vector<Matrix44> misilsTransform;
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
			misilsTransform.push_back(misil.transform);

			std::cout << "Owner X " << this->owner->getGlobalPosition().x << std::endl;

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

	/*
	Mesh mesh1;
	for (int i = 0; i < maxMisil; i++) {
		Bullet& misil = this->misils[i];
		if (misil.timeToLive > 0) {
			mesh1.vertices.push_back(misil.position);
			mesh1.colors.push_back(Vector4(1, 0, 0, 1));
		}
	}

	if (mesh1.vertices.size() > 0) {
		glPointSize(10);
		mesh1.renderFixedPipeline(GL_POINTS);
	}
	*/
	

}

void RocketLauncher::update(float deltaTime) {
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
		misil.velocity = misil.velocity + Vector3(0, -2, 0) * deltaTime; // aplicar gravedad
		misil.velocity = misil.velocity * 0.99999995;
		misil.timeToLive -= deltaTime;

	}

	CollisionHandler::bulletsCollisionAgainstStaticEntities(this->misils, maxMisil);
	CollisionHandler::bulletsCollisionAgainstDynamicEntities(this->misils, maxMisil);
}