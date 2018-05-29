#include "DropBomb.h"
#include "Airplane.h"
#include "Factory.h"
#include "World.h"
#include "CollisionHandler.h"

DropBomb::DropBomb(Airplane* owner, std::string type) : Weapon(owner, type) {
	memset(&this->bombs, 0, sizeof(bombs));
}

void DropBomb::shoot() {
	if (this->cooldown < 0) {

		Matrix44 bombTransform = this->owner->getGlobalMatrix();
		bombTransform.translate(0, -2, 1);

		Vector3 velocity = Vector3(0, -1, 0);
		velocity = velocity * this->bulletSpeed;

		Projectile bomb = Factory::buildProjectile(bombTransform, velocity, 300, this->type, this->owner, this->damage);

		for (int i = 0; i < maxBombs; i++) {
			Projectile& auxBomb = this->bombs[i];
			if (auxBomb.timeToLive > 0) {
				continue;
			}

			auxBomb = bomb;
			break;
		}

		this->cooldown = 1;
	}
}

void DropBomb::render() {

	std::vector<Matrix44> misilsTransform;
	Mesh* mesh = Mesh::Load("data/weapons/torpedo.ASE");
	Shader* shader = Shader::Load("data/shaders/instanced.vs", "data/shaders/texture.fs");
	//Shader* shader = this->meshMisil->material->shader;
	Texture* texture = Texture::Load("data/weapons/torpedo.tga");
	Camera* camera = World::instance->currentCamera;

	if (mesh == NULL || shader == NULL) {
		return;
	}

	for (int i = 0; i < maxBombs; i++) {
		Projectile& bomb = this->bombs[i];

		if (bomb.timeToLive > 0) {
			misilsTransform.push_back(bomb.transform);
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

	Mesh mesh1;

	for (int i = 0; i < maxBombs; i++) {
		Projectile& bomb = this->bombs[i];
		if (bomb.timeToLive > 0) {
			mesh1.vertices.push_back(bomb.position);
			mesh1.colors.push_back(Vector4(1, 0, 0, 1));
		}
	}

	if (mesh1.vertices.size() > 0) {
		glPointSize(30);
		mesh1.renderFixedPipeline(GL_POINTS);
	}

}

void DropBomb::update(float deltaTime) {
	Weapon::update(deltaTime);

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