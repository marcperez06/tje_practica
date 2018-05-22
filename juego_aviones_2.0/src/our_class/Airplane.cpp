#include "Airplane.h"
#include "../input.h"
//#include "CollisionHandler.h"
#include "World.h"
#include "BulletManager.h"
#include "Weapon.h"

// --- CONSTRUCTORES ---

std::vector<Airplane*> Airplane::airplanes;

enum {
	AIRPLANE_FLYING,
	AIRPLANE_CRHASED,
};

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Material * material) : EntityCollider(transform, highMesh, material) {
	this->speed = speed;
	this->health = 100;
	this->state = AIRPLANE_FLYING;
	this->target = NULL;
	airplanes.push_back(this);
}

Airplane::Airplane(float speed, const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material)
					: EntityCollider(transform, highMesh, lowMesh, material) {
	this->speed = speed;
	this->health = 100;
	this->state = AIRPLANE_FLYING;
	this->target = NULL;
	airplanes.push_back(this);
}

Airplane::~Airplane() {
	this->removeAirplane(this);
}

void Airplane::render(Camera* camera) {
	EntityMesh::render(camera);
	for (int i = 0; i < this->weapons.size(); i++) {
		this->weapons[i]->render();
	}
}

void Airplane::update(float deltaTime) {

	if (this->state != AIRPLANE_CRHASED) {

		EntityCollider::update(deltaTime);

		this->transform.translate(Vector3(0, 0, -1) * this->speed * deltaTime);

		if (this->name.compare("player") == 0) {
			this->playerBehaviour(deltaTime);
		} else {
			this->AIBehaviour(deltaTime);
		}

		/* Reduce demasiado el rendimiento... */
		/*
		if (this->detectStaticCollision() == true) {
			std::cout << "Collision !!" << std::endl;
			this->state = AIRPLANE_CRHASED;
		}
		*/
	}

}

void Airplane::playerBehaviour(float deltaTime) {

	float deltaMove = deltaTime * this->speed * 0.02;
	this->rotateAirplane(deltaMove);
	
	this->turbo(deltaTime);

	if (Input::isKeyPressed(SDL_SCANCODE_SPACE) == true) {
		this->shoot();
	}

	for (int i = 0; i < this->weapons.size(); i++) {
		this->weapons[i]->update(deltaTime);
	}

	if (this->detectStaticCollision() == true) {
		std::cout << "Collision !!" << std::endl;
		this->state = AIRPLANE_CRHASED;
	}
}

void Airplane::AIBehaviour(float deltaTime) {

	if (this->target == NULL) {
		return;
	}

	Matrix44 modelInverse = this->getGlobalMatrix();
	modelInverse.inverse();

	Vector3 pos = this->getGlobalPosition();
	Vector3 targetPos = this->target->getGlobalPosition();

	Vector3 toTarget = (targetPos - pos);

	if (abs(toTarget.length()) >= 0.0001) {
		return;
	}

	toTarget.normalize();

	Vector3 front = this->getGlobalMatrix().rotateVector(Vector3(0, 0, -1));
	front.normalize();
	if (abs(front.length()) >= 1.01) {
		return;
	}

	float angle = acos(front.dot(toTarget));

	if (abs(angle) < 0.0000001) {
		return;
	}

	Vector3 axis = front.cross(toTarget);
	axis = modelInverse.rotateVector(axis);
	axis.normalize();

	this->transform.matrixModel.rotate(angle, axis * -1);

}

void Airplane::rotateAirplane(float deltaMove) {
	this->rotateRollDirection(deltaMove);
	this->rotatePitchDirection(deltaMove);
	this->rotateJawDirection(deltaMove);
}

void Airplane::rotateRollDirection(float deltaMove) {
	Vector3 roll = Vector3(0, 0, 1);
	// roll right
	if (Input::isKeyPressed(SDL_SCANCODE_Q) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, roll);
	}

	// roll left
	if (Input::isKeyPressed(SDL_SCANCODE_E) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, roll);
	}
}

void Airplane::rotatePitchDirection(float deltaMove) {
	Vector3 pitch = Vector3(1, 0, 0);
	// down
	if (Input::isKeyPressed(SDL_SCANCODE_S) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, pitch);
	}

	// up
	if (Input::isKeyPressed(SDL_SCANCODE_W) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, pitch);
	}
}

void Airplane::rotateJawDirection(float deltaMove) {
	Vector3 jaw = Vector3(0, 1, 0);
	// jaw right
	if (Input::isKeyPressed(SDL_SCANCODE_D) == true) {
		this->transform.matrixModel.rotate(1 * deltaMove, jaw);
	}

	// jaw left
	if (Input::isKeyPressed(SDL_SCANCODE_A) == true) {
		this->transform.matrixModel.rotate(-1 * deltaMove, jaw);
	}
}

void Airplane::turbo(float deltaTime) {
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) == true) {
		this->transform.translate(Vector3(0, 0, -10) * this->speed * deltaTime);
	}

	if (Input::isKeyPressed(SDL_SCANCODE_TAB) == true) {
		this->transform.translate(Vector3(0, 0, 10) * this->speed * deltaTime);
	}
}

void Airplane::shoot() {	
	this->weapons[currentWepon]->shoot();
}

bool Airplane::detectStaticCollision() {
	bool haveCollision = false;
	Vector3 origin = this->lastPosition;
	Vector3 direction = this->getPosition() - origin;
	return EntityCollider::haveCollisionAgainstStaticObjects(origin, direction);
}

void Airplane::removeAirplane(Airplane* airplane) {
	bool founded = false;
	for (int i = 0; (i < airplanes.size()) && (founded == false); i++) {
		if (airplanes[i] == airplane) {
			airplanes.erase(airplanes.begin() + i);
			founded = true;
		}
	}
}

void Airplane::collisionEffect() {
	this->state = AIRPLANE_CRHASED;
}