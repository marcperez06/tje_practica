#include "HistoryStage.h"
#include "../GUI.h"
#include "../managers/SoundManager.h"
#include "../../game.h"
#include "../../texture.h"
#include "../../input.h"

HistoryStage::HistoryStage() {
	this->gui = new GUI(Game::instance->window_width, Game::instance->window_height);
	this->historyTexture = Texture::Load("data/stages/controls.tga");
	this->numSlide = 1;
	HistoryStage::instance = this;
}

HistoryStage::~HistoryStage() {
	delete this->gui;
	delete this->historyTexture;
	Stage::removeStage("historyStage");
}

void HistoryStage::render() {

	glClearColor(0, 0, 0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	this->gui->drawGUIElement(this->historyTexture, Vector2(0, 0), Vector2(this->gui->windowWidth, this->gui->windowHeight));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void HistoryStage::update(float deltaTime) {
	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
		this->numSlide++;
	} else if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
		this->numSlide--;
	}

	if (this->numSlide < 2) {
		std::string pathTexture = "data/stages/history/history" + std::to_string(this->numSlide);
		pathTexture = pathTexture + ".tga";
		this->historyTexture = Texture::Load(pathTexture.c_str());
	} else {
		Stage::onChange("gameStage");
	}
}