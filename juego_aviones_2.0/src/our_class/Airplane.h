#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "EntityCollider.h"
#include "Weapon.h"

class Airplane : public EntityCollider {

	public:

		static std::vector<Airplane*> airplanes;

		float speed;
		float health;
		char state;
	
		std::vector<Weapon*> weapons;
		int currentWepon;

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

	private:

		void removeAirplane(Airplane* airplane);

};

#endif