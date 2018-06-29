#ifndef BUNKER_H
#define BUNKER_H

#include "EntityCollider.h"

enum BunkerStates {
	BUNKER_LIVING,
	BUNKER_DESTROYED
};

class Bunker : public EntityCollider {

public:

	float health;
	char state;
	char team;
	//Weapon * weapon;

	// --- CONSTRUCTORES ---

	Bunker(const Transform transform, Mesh * highMesh, Material * material, char team);
	Bunker(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material, char team);

	virtual ~Bunker();

	virtual void render(Camera* camera);

	virtual void update(float deltaTime);

	virtual void collisionEffectAgainstDynamicEntity();

	virtual void onBulletCollision(Bullet &  bullet, Vector3 collision);


};

#endif