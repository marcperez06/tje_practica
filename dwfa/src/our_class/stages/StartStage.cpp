#include "StartStage.h"
#include "../GUI.h"
#include "../managers/SoundManager.h"
#include "../../game.h"
#include "../../texture.h"
#include "../../input.h"

StartStage::StartStage() {
	this->gui = new GUI(Game::instance->window_width, Game::instance->window_height);
	this->startTexture = Texture::Load("data/stages/start.tga");
	//SoundManager::reproduceSound("musicmenu.wav");
	StartStage::instance = this;
}

StartStage::~StartStage() {
	delete this->gui;
	delete this->startTexture;
	Stage::removeStage("startStage");
}

void StartStage::render() {

	glClearColor(0, 0, 0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	this->gui->drawGUIElement(this->startTexture, Vector2(0, 0), Vector2(this->gui->windowWidth, this->gui->windowHeight));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void StartStage::update(float deltaTime) {
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) == true || Input::wasButtonPressed(A_BUTTON) == true) {
		Stage::onChange("menuStage");
	} else if (Input::wasKeyPressed(SDL_SCANCODE_RETURN) == true || Input::wasButtonPressed(B_BUTTON) == true) {
		exit(0);
	}
}