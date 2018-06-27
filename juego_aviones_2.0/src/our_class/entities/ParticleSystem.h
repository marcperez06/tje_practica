#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "EntityMesh.h"

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
		};

		float duration;
		float fixedDuration;
		bool looping;
		int maxParticles;
		float generalSpeed;
		Matrix44 ownerModel;

		std::vector<Particle> particles;

		ParticleSystem(Matrix44 model = Matrix44(), int maxParticles = 30, Vector3 direction = Vector3());
		~ParticleSystem();

		virtual void render(Camera* camera);
		virtual void update(float deltaTime, Matrix44 model = Matrix44());

		void initParticles(Vector3 direction);
		void restartParticle(Particle & particle);
		void setTexture(const char * fileTexture);
		void setMaterial(Material* material);

		static ParticleSystem* createExplosion(Matrix44 model, bool looping = false);
		static ParticleSystem* createSmoke(Matrix44 model, bool looping = true);

};

#endif