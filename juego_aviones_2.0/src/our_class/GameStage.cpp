#include "GameStage.h"
#include "GUI.h"
#include "SoundManager.h"

#include "Airplane.h"
#include "World.h"

#include "BulletManager.h"
#include "ProjectileManager.h"

#include "Weapon.h"

#include "..\game.h"
#include "..\rendertotexture.h"
#include "..\input.h"

GameStage::GameStage() {
	this->gameSpeed = 1;
	this->world = new World();
	this->bulletManager = new BulletManager();
	this->projectileManager = new ProjectileManager();
	this->gui = new GUI(Game::instance->window_width, Game::instance->window_height);
	this->rt = new RenderToTexture();
	this->rt->create(612, 612, true);

	this->screenShader = Shader::Load("data/shaders/screen.vs", "data/shaders/screen.fs");

	GameStage::instance = this;
}

GameStage::~GameStage() {
	delete this->gui;
	delete this->bulletManager;
	delete this->projectileManager;
	delete this->screenShader;
	delete this->world;
	Stage::removeStage("gameStage");
}

void GameStage::render() {

	//set the clear color (the background color)
	float red = 161.0 / 255.0;
	float green = 192.0 / 255.0;
	float blue = 203.0 / 255.0;
	glClearColor(red, green, blue, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	this->world->currentCamera->enable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	this->rt->enable();

	glClearColor(red, green, blue, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->world->render(this->world->currentCamera);

	this->rt->disable();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	this->screenShader->enable();
	this->screenShader->setUniform("u_time", Game::instance->time);
	this->screenShader->setUniform("texture_size", 612);
	this->rt->toViewport(screenShader);
	this->screenShader->disable();

	for (int i = 0; i < world->AIAirplanes.size(); i++) {

		if (this->world->player->team == this->world->AIAirplanes[i]->team) {
			continue;
		}

		if (World::distanceBetween(this->world->player, this->world->AIAirplanes[i]) > 200) {
			this->gui->highlightEntity(this->world->AIAirplanes[i]);
		}

	}

	this->gui->render();

	//Draw out world
	//drawGrid();

	//render the FPS
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	glDisable(GL_DEPTH_TEST);
}

void GameStage::update(float deltaTime) {
	

	float speed = deltaTime * 10; //the speed is defined by the seconds_elapsed so it goes constant

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || Game::instance->mouse_locked) //is left button pressed?
	{
		this->world->freeCamera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		this->world->freeCamera->rotate(Input::mouse_delta.y * 0.005f, this->world->freeCamera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 50; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) this->world->freeCamera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) this->world->freeCamera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) this->world->freeCamera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) this->world->freeCamera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

	if (Input::wasKeyPressed(SDL_SCANCODE_1) == true) {
		this->world->currentCamera = (this->world->currentCamera == this->world->freeCamera) ? this->world->playerCamera : this->world->freeCamera;
		this->world->freeCamera->eye = this->world->playerCamera->eye;
		this->gameSpeed = (this->world->currentCamera == this->world->playerCamera) ? 1 : 0.01;
	} else if (Input::wasKeyPressed(SDL_SCANCODE_2) == true) {
		this->world->player->state = 0;
		this->world->player->health = 100;
		this->world->player->fuell = 500;
	}

	this->world->update(deltaTime * this->gameSpeed);

}