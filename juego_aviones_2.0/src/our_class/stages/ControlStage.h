#ifndef CONTROL_STAGE_H
#define CONTROL_STAGE_H

#include "Stage.h"

class GUI;
class Texture;

class ControlStage : public Stage {
	public:
		GUI* gui;
		Texture* controlTexture;

		ControlStage();
		~ControlStage();

		virtual void render();
		virtual void update(float deltaTime);
};

#endif