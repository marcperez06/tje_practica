#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"

#include <cmath>

#include "our_class\MenuStage.h"
#include "our_class\ControlStage.h"
#include "our_class\HistoryStage.h"
#include "our_class\GameStage.h"
#include "our_class\EndStage.h"
#include "our_class\World.h"

#include "rendertotexture.h"
#include "bass.h"

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;

float gameSpeed;

Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	
	gameSpeed = 1;

	Stage::addStage("menuStage", new MenuStage());
	Stage::addStage("controlStage", new ControlStage());
	Stage::addStage("historyStage", new HistoryStage());
	Stage::addStage("gameStage", new GameStage());
	Stage::addStage("endStage", new EndStage());

	Stage::instance->current = Stage::stages["menuStage"];

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{

	Stage::instance->current->render();

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{

	Stage::instance->current->update(seconds_elapsed);

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();

}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	World::instance->freeCamera->aspect =  width / (float)height;
	World::instance->playerCamera->aspect = width / (float)height;
	window_width = width;
	window_height = height;
}