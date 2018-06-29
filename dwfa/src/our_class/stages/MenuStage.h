#ifndef MENU_STAGE_H
#define MENU_STAGE_H

#include "Stage.h"

class GUI;
class Texture;

class MenuStage : public Stage {
	public:
		GUI* gui;
		Texture* menuTexture;

		MenuStage();
		~MenuStage();

		virtual void render();
		virtual void update(float deltaTime);
};

#endif