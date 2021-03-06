#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "EntityMesh.h"

enum ParticleType {
	SMOKE,
	EXPLOSION

};

const unsigned maxParticles = 50;

class ParticleSystem : EntityMesh {

	public:
		struct Particle {
			int id;
			Vector3 pos;
			Vector3 direction;
			float size;
			float distance;
			float speed;
			float timeToLive;
			char type;
		};

		float duration;
		float fixedDuration;
		bool looping;
		float generalSpeed;
		Matrix44 ownerModel;

		Particle particles[maxParticles];

		ParticleSystem(Matrix44 model = Matrix44(), Vector3 direction = Vector3());
		~ParticleSystem();

		virtual void render(Camera* camera);
		virtual void update(float deltaTime, Matrix44 model = Matrix44());

		void initParticles(Vector3 direction);
		void restartParticle(Particle & particle);
		void setTexture(const char * fileTexture);
		void setMaterial(Material* material);

		void clearParticles();
		int particlesSize();

		static ParticleSystem* createExplosion(Matrix44 model, bool looping = false);
		static ParticleSystem* createSmoke(Matrix44 model, bool looping = true);

};

#endif