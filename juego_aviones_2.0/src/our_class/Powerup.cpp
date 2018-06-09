#include "Powerup.h"
#include "World.h"
#include "Airplane.h"
#include "Factory.h"
#include "ShootGun.h"
#include "DropBomb.h"
#include "RocketLauncher.h";

std::vector<Powerup*> Powerup::powerups;

Powerup::Powerup(char type, Vector3 pos) : EntityCollider() {
	Transform transform = Transform(pos, Quaternion());
	Material* material = new Material(NULL, NULL, Vector4(1, 0, 1, 0));
	this->transform = transform;
	this->material = material;
	
	this->powerupType = type;
	powerups.push_back(this);
}

Powerup::~Powerup() {
	this->removePowerup(this);
}

void Powerup::removePowerup(Powerup* powerup) {
	bool founded = false;
	for (int i = 0; (i < powerups.size()) && (founded == false); i++) {
		if (powerups[i] == powerup) {
			powerups.erase(powerups.begin() + i);
			founded = true;
		}
	}
}

void Powerup::render(Camera* camera) {
	Mesh mesh;

	mesh.vertices.push_back(this->getGlobalPosition());
	mesh.colors.push_back(this->material->color);

	if (mesh.vertices.size() > 0) {
		glPointSize(20);
		mesh.renderFixedPipeline(GL_POINTS);
	}
}

void Powerup::update(float deltaTime) {
	Airplane* player = World::instance->player;
	if (this->detectCollision((Entity*) player) == true) {
		
		if (player->type == AIRPLANE) {
			this->onAirplaneCollision(player);
		}

		removePowerup(this);
	}
}

bool Powerup::detectCollision(Entity* entity) {
	float distance = World::distanceBetween(this, entity);
	return (distance < 10);
}

void Powerup::onAirplaneCollision(Airplane* airplane) {

	if (this->powerupType == POWERUP_ROCKET_LAUNCHER) {
		Weapon* weapon = airplane->getWeapon(ROCKET_LAUNCHER);
		if (weapon != NULL) {
			weapon->addAmmounition(5);
		}
		else {
			RocketLauncher* rocketLauncer = Factory::buildRocketLauncher(airplane);
			rocketLauncer->addAmmounition(10);
			airplane->weapons.push_back(rocketLauncer);
		}
	} else 	if (this->powerupType == POWERUP_DROP_BOMB) {
		Weapon* weapon = airplane->getWeapon(DROP_BOMB);
		if (weapon != NULL) {
			weapon->addAmmounition(5);
		} else {
			DropBomb* dropBomb = Factory::buildDropBomb(airplane);
			dropBomb->addAmmounition(5);
			airplane->weapons.push_back(dropBomb);
		}
	} else if (this->powerupType == POWERUP_SHOOT_GUN) {
		ShootGun* shootGun = Factory::buildShootGun(airplane);
		airplane->weapons.push_back(shootGun);
	} else if (this->powerupType == POWERUP_HEALTH) {
		airplane->health += 50;
	} else if (this->powerupType == POWERUP_FUELL) {

	}

}