#ifndef WORLD_H
#define WORLD_H

#include "../framework.h"
class Entity;
class EntityMesh;
class Airplane;
class Camera;

class World {
	public:

		static World* instance;

		// De fet les cameres no se si tindrien que anar aqui o no, de moment esta adaptat a que funcioni la camera rebentla per parametre.
		Camera* freeCamera;
		Camera* playerCamera;
		Camera* currentCamera;

		Entity* root;

		Airplane* player;
		std::vector<Airplane*> enemies;

		int numEnemies;

		EntityMesh* sky;
		EntityMesh* sea;

		// Vectores para almacenar las entidades, para comprobar collisiones.
		std::vector<Entity*> staticObjects;
		std::vector<Entity*> dynamicObjects;

		World();
		~World();

		Entity* getWorldMap();

		void render(Camera * camera);
		void update(float deltaTime);

		static bool entityACanSeeEntityB(Entity* entityA, Entity* entityB);
		static bool isEntityANearEntityB(Entity* entityA, Entity* entityB);
		static Vector3 directionEntityAToEntityB(Entity* entityA, Entity* entityB);
		static float distanceBetween(Entity* entityA, Entity* entityB);
		static float angleBetween(Entity* entityA, Entity* entityB);

	private:
		
		void initCameras();
		void initPlayer();
		void initWorldMap();
		void initEnemies();
		void initTeamAlfa();
		void initTeamDelta();
		void initTeamBeta();
		void initTeamGamma();
		void initSky();
		void initSea();

		void renderWorldMap(Camera* camera);
		void renderWater(Camera* camera);
		void renderAirplanes(Camera* camera);

};


#endif