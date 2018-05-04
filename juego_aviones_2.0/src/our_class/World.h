#ifndef WORLD_H
#define WORLD_H

#include "../framework.h"

#include "Airplane.h"
class World
{
	public:

		// De fet les cameres no se si tindrien que anar aqui o no, de moment esta adaptat a que funcioni la camera rebentla per parametre.
		//Camera* freeCam;
		//Camera* playerCam;

		Airplane* player;
		int numEnemies;
		std::vector<Airplane*> enemies;
		Entity* worldMap; // Entity que conté totes les illes (dins de la classe Entity hi ha un vector que s'utilitza per crear un arbre de Entitats)
		EntityMesh* sky;

		World();
		~World();

		void render(Camera * camera);
		void update(float deltaTime);

	private:
		
		//void initCameras();
		void initPlayer();
		void initWorldMap();
		void initEnemies();

};


#endif