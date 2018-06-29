#include "EndStage.h"
#include "../GUI.h"
#include "../managers/SoundManager.h"
#include "../../game.h"
#include "../../texture.h"
#include "../../input.h"
#include "GameStage.h"

EndStage* EndStage::instance = NULL;

EndStage::EndStage() {
	this->gui = new GUI(Game::instance->window_width, Game::instance->window_height);
	this->success = false;
	this->gameOverTexture = Texture::Load("data/stages/gameover.tga");
	this->successTexture = Texture::Load("data/stages/success.tga");
	EndStage::instance = this;
}

EndStage::~EndStage() {
	delete this->gui;
	delete this->gameOverTexture;
	delete this->successTexture;
	Stage::removeStage("endStage");
}

void EndStage::render() {

	glClearColor(0, 0, 0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Texture* endTexture = this->gameOverTexture;
	if (this->success == true) {
		endTexture = this->successTexture;
	}

	this->gui->drawGUIElement(endTexture, Vector2(0, 0), Vector2(this->gui->windowWidth, this->gui->windowHeight));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void EndStage::update(float deltaTime) {
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) == true 
		|| Input::wasButtonPressed(A_BUTTON) == true) {
		
		GameStage::instance->reset();
		Stage::onChange("menuStage");
		this->success = false;
	}
}

void EndStage::lose() {
	SoundManager::reproduceSound("gameover.wav");
	this->instance->success = false;
	this->onChange("endStage");
}

void EndStage::win() {
	SoundManager::reproduceSound("success.wav");
	this->instance->success = true;
	this->onChange("endStage");
}