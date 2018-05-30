#ifndef GUI_H
#define GUI_H

#include "../framework.h"

class Texture;

class GUI {

	public:
		
		GUI();
		~GUI();
		void render();
		void drawGUIElement(Texture* texture, Vector2 start, Vector2 size);

	private:

		void drawPlayerWeapons();
		void drawMachineGun();
		void drawMisil();
		void drawDropBomb();
		void drawShootGun();
		void drawMachineGunFree();
		void drawPlayerHealth();

		void drawCrosshair();


};

#endif
