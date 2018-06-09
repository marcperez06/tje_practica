#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "EntityCollider.h"
#include "Path.h"

class Weapon;
class AirplaneController;
class Weapon;

enum States {
	AIRPLANE_FLYING,
	AIRPLANE_HURT,
	AIRPLANE_CRASHED,
	AIRPLANE_DESTROYED
};

enum Team {
	TEAM_DELTA,
	TEAM_ALFA,
	TEAM_BETA,
	TEAM_GAMMA
};

class Airplane : public EntityCollider {

	public:

		static std::vector<Airplane*> airplanes;
		static std::vector<Airplane*> airplanesToDestroy;

		bool isPlayer;
		float speed;
		float health;
		char state;
		char team;
	
		std::vector<Weapon*> weapons;
		int currentWepon;

		Entity* target;
		AirplaneController* controller;
		Path path;

		// --- CONSTRUCTORES ---

		Airplane(const float speed, const Transform transform, Mesh * highMesh, Material * material);
		Airplane(const float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material);

		virtual ~Airplane();

		virtual void render(Camera* camera);

		virtual void update(float deltaTime);

		void rotateAirplane(float deltaMove);
		void rotateRollDirection(float deltaMove);
		void rotatePitchDirection(float deltaMove);
		void rotateYawDirection(float deltaMove);

		void turbo(float deltaTime);

		void selectWeapon(char type);
		void shoot();
		Weapon* getWeapon(char type);

		bool detectStaticCollision();

		virtual void collisionEffect();

		virtual void onBulletCollision(Bullet &  bullet, Vector3 collision);

		static void destroyDeadAirplanes();

	private:

		void removeAirplane(Airplane* airplane);
		void removeAirplaneToDestroy(Airplane* airplane);
		void removeWeapons();
};

#endif