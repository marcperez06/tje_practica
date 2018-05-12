#ifndef WORLD_H
#define WORLD_H

#include "../framework.h"

#include "Airplane.h"
class World
{
	public:

		static World* instance;

		// De fet les cameres no se si tindrien que anar aqui o no, de moment esta adaptat a que funcioni la camera rebentla per parametre.
		Camera* freeCamera;
		Camera* playerCamera;
		Camera* currentCamera;

		Entity* root;

		Airplane* player;
		int numEnemies;

		EntityMesh* sky;

		World();
		~World();

		Entity* getWorldMap();

		void render(Camera * camera);
		void update(float deltaTime);

	private:
		
		void initCameras();
		void initPlayer();
		void initWorldMap();
		void initEnemies();
		void initSky();

		void renderWorldMap(Camera* camera);
		void renderAirplanes(Camera* camera);

};


#endif