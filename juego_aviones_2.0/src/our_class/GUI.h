#ifndef GUI_H
#define GUI_H

#include "../framework.h"

class Texture;
class Entity;
class Shader;

class GUI {

	public:

		float windowWidth;
		float windowHeight;
		Shader* shader;
		
		GUI(float windowWidth, float windowHeight);
		~GUI();
		void render();
		void drawGUIElement(Texture* texture, Vector2 start, Vector2 size);

		void highlightEntity(Entity* entity);

	private:

		void drawPlayerWeapons();
		void drawMachineGun();
		void drawMisil();
		void drawDropBomb();
		void drawShootGun();
		void drawMachineGunFree();
		void drawPlayerHealth();
		void drawPlayerFuell();

		void drawCrosshair();


};

#endif
