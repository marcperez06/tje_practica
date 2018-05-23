#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "EntityCollider.h"

class Weapon;

class Airplane : public EntityCollider {

	public:

		static std::vector<Airplane*> airplanes;

		float speed;
		float health;
		char state;
	
		std::vector<Weapon*> weapons;
		int currentWepon;

		Airplane* target;

		// --- CONSTRUCTORES ---

		Airplane(const float speed, const Transform transform, Mesh * highMesh, Material * material);
		Airplane(const float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		virtual ~Airplane();

		virtual void render(Camera* camera);

		virtual void update(float deltaTime);

		void rotateAirplane(float deltaMove);
		void rotateRollDirection(float deltaMove);
		void rotatePitchDirection(float deltaMove);
		void rotateJawDirection(float deltaMove);

		void turbo(float deltaTime);

		void shoot();

		bool detectStaticCollision();

		virtual void collisionEffect();

		virtual void onBulletCollision(Bullet &  bullet, Vector3 collision);

	private:

		void removeAirplane(Airplane* airplane);
		void AIBehaviour(float deltaTime);
		void playerBehaviour(float deltaTime);

};

#endif