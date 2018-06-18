#include "Bunker.h"
//#include "CollisionHandler.h"
#include "World.h"

#include "BulletManager.h"
#include "SoundManager.h"


// --- CONSTRUCTORES ---

Bunker::Bunker(const Transform transform, Mesh * highMesh, Material * material, char team) : EntityCollider(transform, highMesh, material) {
	
	this->health = 1000;
	this->state = BUNKER_LIVING;
	this->team = team;
	this->type = MILITARY_BASE;
}

Bunker::Bunker(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material, char team) : EntityCollider(transform, highMesh, lowMesh, material) {

	this->health = 1000;
	this->state = BUNKER_LIVING;
	this->team = team;
	this->type = MILITARY_BASE;
}

Bunker::~Bunker() {
	delete this;
}


void Bunker::render(Camera* camera) {
	EntityMesh::render(camera);
}

void Bunker::update(float deltaTime) {

	if (this->state != BUNKER_DESTROYED) {

		EntityCollider::update(deltaTime);

		if (this->health <= 0) {
			this->state = BUNKER_DESTROYED;
		}

	}

}

bool Bunker::detectStaticCollision() {
	bool haveCollision = false;
	Vector3 origin = this->lastPosition;
	Vector3 direction = this->getPosition() - origin;
	return EntityCollider::haveCollisionAgainstStaticObjects(origin, direction);
}

void Bunker::onBulletCollision(Bullet & bullet, Vector3 collision) {

	std::cout << "SOME ONE HIT BUNKER!!!!!!!! " << std::endl;
	Vector3 globalCollision = this->getGlobalMatrix() * collision;

	std::cout << "Collision Y !!!!!!!! " << globalCollision.y << std::endl;

	this->health -= bullet.damage;

	//SoundManager::reproduceSound("damage.wav");

	Mesh mesh;
	mesh.vertices.push_back(this->getGlobalMatrix() * collision);
	mesh.colors.push_back(Vector4(0, 0, 1, 1));
	glPointSize(500);
	mesh.renderFixedPipeline(GL_POINTS);
}

void Bunker::collisionEffect() {
	this->material->color = Vector4(0, 0, 0, 1);
	//this->state = AIRPLANE_DESTROYED;
	//airplanesToDestroy.push_back(this);
}