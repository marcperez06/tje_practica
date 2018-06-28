#ifndef WORLD_H
#define WORLD_H

#include "../framework.h"
class Entity;
class EntityMesh;
class Airplane;
class Camera;
class Clouds;
class Bunker;
class ParticleSystem;

enum Team {
	TEAM_DELTA,
	TEAM_ALFA,
	TEAM_BETA,
	TEAM_GAMMA
};

class World {
	public:

		static World* instance;

		// De fet les cameres no se si tindrien que anar aqui o no, de moment esta adaptat a que funcioni la camera rebentla per parametre.
		Camera* freeCamera;
		Camera* playerCamera;
		Camera* currentCamera;

		Entity* root;

		Airplane* player;
		std::vector<Airplane*> AIAirplanes;

		std::vector<Bunker*> teamMilitaryBases;
		//std::vector<Powerup*> powerups;

		int hardFactor;
		int numAIAirplanes;
		int numOfTeams;

		EntityMesh* sky;
		EntityMesh* sea;
		Clouds* clouds;

		// Vectores para almacenar las entidades, para comprobar collisiones.
		std::vector<Entity*> staticObjects;
		std::vector<Entity*> dynamicObjects;

		World(int hardFactor = 1);
		~World();

		Entity* getWorldMap();

		void render(Camera * camera);
		void update(float deltaTime);

		static bool entityACanSeeEntityB(Entity* entityA, Entity* entityB);
		static bool isEntityANearEntityB(Entity* entityA, Entity* entityB);
		static Vector3 directionEntityAToEntityB(Entity* entityA, Entity* entityB);
		static float distanceBetween(Entity* entityA, Entity* entityB);
		static float angleBetween(Entity* entityA, Entity* entityB);

		template<class T> static void removeElement(std::vector<T> list, T element);
		template<class T> static void deleteAndRemoveElement(std::vector<T> list, T element);

		void createRandomPowerup();
		void createPowerup(char type);

	private:
		
		void initCameras();
		void initPlayer();
		void initWorldMap();
		void initTeams();
		void initTeamAlfa();
		void initTeamDelta();
		void initTeamBeta();
		void initTeamGamma();
		void initSky();
		void initSea();
		void initBunkers();

		void renderWorldMap(Camera* camera);
		void renderWater(Camera* camera);
		void renderAirplanes(Camera* camera);
		void renderPowerups(Camera* camera);
		void renderBunkers(Camera* camera);

		void updatePowerups(float deltaTime);

		void cameraFollowEntity(Camera* camera, Entity* entity);

};


#endif


