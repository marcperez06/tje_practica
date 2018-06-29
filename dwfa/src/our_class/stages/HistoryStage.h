#ifndef HISTORY_STAGE_H
#define HISTORY_STAGE_H

#include "Stage.h"

class GUI;
class Texture;

class HistoryStage : public Stage {
	public:
		GUI* gui;
		Texture* historyTexture;
		int numSlide;

		HistoryStage();
		~HistoryStage();

		virtual void render();
		virtual void update(float deltaTime);
};

#endif