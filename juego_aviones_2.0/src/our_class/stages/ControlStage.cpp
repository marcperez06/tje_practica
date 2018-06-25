#include "ControlStage.h"
#include "../GUI.h"
#include "../managers/SoundManager.h"
#include "../../game.h"
#include "../../texture.h"
#include "../../input.h"

ControlStage::ControlStage() {
	this->gui = new GUI(Game::instance->window_width, Game::instance->window_height);
	this->controlTexture = Texture::Load("data/stages/controls.tga");
	ControlStage::instance = this;
}

ControlStage::~ControlStage() {
	delete this->gui;
	delete this->controlTexture;
	Stage::removeStage("controlStage");
}

void ControlStage::render() {

	glClearColor(0, 0, 0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	this->gui->drawGUIElement(this->controlTexture, Vector2(0, 0), Vector2(this->gui->windowWidth, this->gui->windowHeight));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void ControlStage::update(float deltaTime) {
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) == true
		|| Input::wasKeyPressed(SDL_SCANCODE_RETURN) == true
		|| Input::wasButtonPressed(B_BUTTON) == true) {
		Stage::onChange("menuStage");
	}
}