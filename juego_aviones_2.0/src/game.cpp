#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"

#include <cmath>

#include "our_class\Airplane.h"
#include "our_class\World.h"
#include "our_class\BulletManager.h"

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;

World* world = NULL;
BulletManager* bulletManager = NULL;

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

	// Build world.
	world = new World();
	bulletManager = new BulletManager();

	/*

	//create a plane mesh
	mesh = Mesh::Load("data/island/island.ASE");

	//load one texture
	texture = new Texture();
 	texture->load("data/island/island_color.tga");

	// example of shader loading
	shader = Shader::Load("data/shaders/basic.vs", "data/shaders/texture.fs");

	*/

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	float red = 161.0 / 255.0;
	float green = 192.0 / 255.0;
	float blue = 203.0 / 255.0;
	glClearColor(red, green, blue, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	world->currentCamera->enable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	world->render(world->currentCamera);

	//Draw out world
	drawGrid();

	//render the FPS
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	glDisable(GL_DEPTH_TEST);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);

	/*
	
	//create model matrix for cube
	Matrix44 m;
	m.rotate( (float)(angle * DEG2RAD), Vector3(0.0f,1.0f, 0.0f) ); //build a rotation matrix

	Shader* current_shader = shader;

	if(current_shader)
	{
		//enable shader
		current_shader->enable();

		//upload uniforms
		current_shader->setUniform("u_color", Vector4(1,1,1,1));
		current_shader->setUniform("u_viewprojection", freeCamera->viewprojection_matrix );
		current_shader->setUniform("u_texture", texture);
		current_shader->setUniform("u_model", m);
		current_shader->setUniform("u_time", time);

		//current_shader->setUniform("u_model", m);
		mesh->render(GL_TRIANGLES, current_shader);

		//disable shader
		current_shader->disable();
	}
   
	
	player->render(currentCamera);

	*/

}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * 10; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		World::instance->freeCamera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		World::instance->freeCamera->rotate(Input::mouse_delta.y * 0.005f, World::instance->freeCamera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around
	if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 50; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) World::instance->freeCamera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) World::instance->freeCamera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) World::instance->freeCamera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) World::instance->freeCamera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);

	world->update(seconds_elapsed * gameSpeed);
	
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
		case SDLK_1:
			World::instance->currentCamera = (World::instance->currentCamera == World::instance->freeCamera) ? World::instance->playerCamera : World::instance->freeCamera;
			World::instance->freeCamera->eye = World::instance->playerCamera->eye;
			gameSpeed = (World::instance->currentCamera == World::instance->playerCamera) ? 1 : 0.01;
			break;
		case SDLK_2: World::instance->player->state = 0;
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