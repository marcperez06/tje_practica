#ifndef START_STAGE_H
#define START_STAGE_H

#include "Stage.h"

class GUI;
class Texture;

class StartStage : public Stage {
public:
	GUI* gui;
	Texture* startTexture;

	StartStage();
	~StartStage();

	virtual void render();
	virtual void update(float deltaTime);
};

#endif