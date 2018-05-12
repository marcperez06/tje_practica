#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "EntityMesh.h"
#include "Weapon.h"

class Airplane : public EntityMesh {

	public:

		static std::vector<Airplane*> airplanes;

		float speed;
		float health;
	
		std::vector<Weapon*> weapons;
		int currentWepon;

		// --- CONSTRUCTORES ---

		Airplane(const float speed, const Vector3 position);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Material * material);
		Airplane(const float speed, const Transform transform, Mesh * highMesh, Material * material);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh);
		Airplane(const float speed, const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color);
		Airplane(const float speed, const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh, Material * material);
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

		bool detectCollision();

	private:

		void removeAirplane(Airplane* airplane);

};

#endif