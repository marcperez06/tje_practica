#ifndef GAME_STAGE_H
#define GAME_STAGE_H

#include "Stage.h"

class World;
class BulletManager;
class ProjectileManager;
class GUI;
class RenderToTexture;
class Shader;

class GameStage : public Stage {
	public:
		static GameStage* instance;
		World* world;
		BulletManager* bulletManager;
		ProjectileManager* projectileManager;
		GUI* gui;
		RenderToTexture* rt;
		Shader* screenShader;

		float gameSpeed;

		GameStage();
		~GameStage();

		virtual void render();
		virtual void update(float deltaTime);

		void reset();
};

#endif