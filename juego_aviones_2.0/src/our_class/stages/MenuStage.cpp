#include "MenuStage.h"
#include "../GUI.h"
#include "../managers/SoundManager.h"
#include "../../game.h"
#include "../../texture.h"
#include "../../input.h"
#include "GameStage.h"

MenuStage::MenuStage() {
	this->gui = new GUI(Game::instance->window_width, Game::instance->window_height);
	this->menuTexture = Texture::Load("data/stages/menu.tga");
	SoundManager::reproduceSound("musicmenu.wav");
	MenuStage::instance = this;
}

MenuStage::~MenuStage() {
	delete this->gui;
	delete this->menuTexture;
	Stage::removeStage("menuStage");
}

void MenuStage::render() {

	glClearColor(0, 0, 0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	this->gui->drawGUIElement(this->menuTexture, Vector2(0, 0), Vector2(this->gui->windowWidth, this->gui->windowHeight));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void MenuStage::update(float deltaTime) {
	if (Input::wasKeyPressed(SDL_SCANCODE_1) == true || Input::wasButtonPressed(A_BUTTON) == true) {
		GameStage::instance->hardFactor = 1;
		Stage::onChange("historyStage");
		GameStage::instance->restart();
	} else if (Input::wasKeyPressed(SDL_SCANCODE_2) == true || Input::wasButtonPressed(Y_BUTTON) == true) {
		GameStage::instance->hardFactor = 2;
		Stage::onChange("historyStage");
		GameStage::instance->restart();
	} else if (Input::wasKeyPressed(SDL_SCANCODE_3) == true || Input::wasButtonPressed(X_BUTTON) == true) {
		Stage::onChange("controlStage");
	} else if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE) == true || Input::wasButtonPressed(B_BUTTON) == true) {
		exit(0);
	}
}