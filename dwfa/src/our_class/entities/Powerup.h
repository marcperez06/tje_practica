#ifndef POWERUP_H
#define POWERUP_H
#pragma once

#include "EntityCollider.h"

class Airplane;

enum PowerupType {
	POWERUP_HEALTH,
	POWERUP_FUELL,
	POWERUP_SHOOT_GUN,
	POWERUP_ROCKET_LAUNCHER,
	POWERUP_DROP_BOMB
};

class Powerup : public EntityCollider {

	public:

		static std::vector<Powerup*> powerups;
		
		char powerupType;

		Powerup(char type, Vector3 pos);
		~Powerup();

		virtual void render(Camera* camera);

		virtual void update(float deltaTime);

		bool detectCollision(Entity* entity);

		void onAirplaneCollision(Airplane* airplane);


	private:
		static void removePowerup(Powerup* powerup);
};

#endif
