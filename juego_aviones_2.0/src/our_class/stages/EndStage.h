#ifndef END_STAGE_H
#define END_STAGE_H

#include "Stage.h"

class GUI;
class Texture;

class EndStage : public Stage {
	public:
		static EndStage* instance;
		GUI* gui;
		Texture* gameOverTexture;
		Texture* successTexture;
		bool success;

		EndStage();
		~EndStage();

		virtual void render();
		virtual void update(float deltaTime);
};

#endif