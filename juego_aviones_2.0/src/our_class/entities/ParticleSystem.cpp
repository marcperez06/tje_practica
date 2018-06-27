#include "ParticleSystem.h"
#include "../World.h"
#include "Material.h"
#include "../../game.h"
#include <algorithm>

bool sortParticleSystem(const ParticleSystem::Particle & particleA, const ParticleSystem::Particle & particleB) {
	return !(particleA.distance <= particleB.distance);
}

ParticleSystem::ParticleSystem(Matrix44 model, int maxParticles, Vector3 direction) : EntityMesh() {
	this->maxParticles = maxParticles;
	this->generalSpeed = 10;
	this->duration = 0;
	this->fixedDuration = 0;
	this->looping = false;
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::initParticles(Vector3 direction) {
	this->particles.clear();
	this->particles.resize(this->maxParticles);
	for (int i = 0; i < this->particles.size(); i++) {
		Particle & particle = this->particles[i];

		particle.id = i;
		Vector3 desviation = Vector3();
		desviation.random(Vector3(0.25, 0, 0.25));
		particle.direction = direction + desviation;

		this->restartParticle(particle);
	}
}

void ParticleSystem::restartParticle(Particle & particle) {
	particle.pos = Vector3();
	particle.size = 5 + (random() * 2);
	particle.timeToLive = 1;
	particle.speed = this->generalSpeed;
}

void ParticleSystem::render(Camera* camera) {

	if (this->particles.size() <= 0) {
		return;
	}

	Shader* shader = this->material->shader;
	Texture* texture = this->material->texture;

	Vector3 cameraPos = camera->eye;
	Vector3 top = camera->getLocalVector(Vector3(0, 1, 0));
	Vector3 right = camera->getLocalVector(Vector3(1, 0, 0));

	Mesh mesh;

	for (int i = 0; i < this->particles.size(); i++) {
		this->particles[i].distance = this->particles[i].pos.distance(cameraPos);
	}

	try {
		std::sort(this->particles.begin(), this->particles.end(), sortParticleSystem);
	} catch (std::exception e) {}

	for (int i = 0; i < this->particles.size(); i++) {

		Particle & particle = this->particles[i];

		//if (camera->testSphereInFrustum(particle.pos, 250) == false || particle.timeToLive <= 0) {
			//continue;
		//}

		Vector2 offset(0, 0);

		if (particle.id % 4 == 1) {
			offset.x = 0.5;
		}
		else if (particle.id % 4 == 2) {
			offset.y = 0.5;
		}
		else if (particle.id % 4 == 3) {
			offset.x = 0.5;
			offset.y = 0.5;
		}

		Vector3 topRight = particle.pos + (top + right) * particle.size;
		Vector3 topLeft = particle.pos + (top - right) * particle.size;
		Vector3 bottomRight = particle.pos + (-1 * top + right) * particle.size;
		Vector3 bottomLeft = particle.pos + (-1 * top - right) * particle.size;

		mesh.vertices.push_back(topLeft);
		mesh.uvs.push_back(Vector2(0, 0) + offset);
		mesh.vertices.push_back(topRight);
		mesh.uvs.push_back(Vector2(0.5, 0) + offset);
		mesh.vertices.push_back(bottomRight);
		mesh.uvs.push_back(Vector2(0.5, 0.5) + offset);

		mesh.vertices.push_back(bottomRight);
		mesh.uvs.push_back(Vector2(0.5, 0.5) + offset);
		mesh.vertices.push_back(bottomLeft);
		mesh.uvs.push_back(Vector2(0, 0.5) + offset);
		mesh.vertices.push_back(topLeft);
		mesh.uvs.push_back(Vector2(0, 0) + offset);

	}

	if (mesh.vertices.size() > 0) {
		shader->enable();

		shader->setUniform("u_color", this->material->color);
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", this->material->texture);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", Game::instance->time);

		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		mesh.render(GL_TRIANGLES, shader);

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		shader->disable();
	}
}

void ParticleSystem::update(float deltaTime, Matrix44 model) {

	this->ownerModel = model;

	if (this->particles.size() <= 0) {
		return;
	}

	this->duration -= deltaTime;

	if (this->duration > 0) {

		for (int i = 0; i < this->particles.size(); i++) {

			Particle & particle = this->particles[i];

			if (particle.timeToLive < 0) {
				if (this->duration > 0) {
					this->restartParticle(particle);
				}
				continue;
			}

			particle.timeToLive -= deltaTime;

			particle.pos = model * particle.pos;

			particle.pos = particle.pos + particle.direction * particle.speed * deltaTime;

		}

	} else {
		if (this->looping == true) {
			this->duration = this->fixedDuration;
		} else {
			for (int i = 0; i < this->particles.size(); i++) {
				Particle & particle = this->particles[i];
				particle.timeToLive = 0;
			}
		}
	}

}

void ParticleSystem::setTexture(const char * fileName) {
	Texture * texture = Texture::Load(fileName);
	this->material->texture = texture;
}

void ParticleSystem::setMaterial(Material* material) {
	this->material = material;
}

ParticleSystem* ParticleSystem::createExplosion(Matrix44 model, bool looping) {
	Texture* texture = Texture::Load("data/clouds/clouds.tga");
	Shader* shader = Shader::Load("data/shaders/clouds.vs", "data/shaders/clouds.fs");
	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1));

	ParticleSystem* explosion = new ParticleSystem(model, 20);
	explosion->duration = 10;
	explosion->fixedDuration = explosion->duration;
	explosion->setMaterial(material);
	explosion->looping = looping;
	explosion->initParticles(model.rotateVector(Transform::UP));
	return explosion;
}

ParticleSystem* ParticleSystem::createSmoke(Matrix44 model, bool looping) {
	ParticleSystem* smoke = new ParticleSystem(model, 20);
	smoke->duration = 10;
	smoke->fixedDuration = smoke->duration;
	Texture* texture = Texture::Load("data/clouds/clouds.tga");
	Shader* shader = Shader::Load("data/shaders/clouds.vs", "data/shaders/clouds.fs");

	Material* material = new Material(texture, shader, Vector4(1, 1, 1, 1));
	
	smoke->looping = looping;

	return smoke;
}