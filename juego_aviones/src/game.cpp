#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"

#include <cmath>

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;
FBO fbo;

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

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	//create a plane mesh
	mesh = Mesh::Load("data/box.ASE");

	//load one texture
	texture = new Texture();
 	texture->load("data/texture.tga");

	// example of shader loading
	shader = Shader::Load("data/shaders/texture.vs", "data/shaders/texture.fs");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
   
	//create model matrix for cube
	Matrix44 m;
	m.rotate( (float)(angle * DEG2RAD), Vector3(0.0f,1.0f, 0.0f) ); //build a rotation matrix

	if(shader) 
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1,1,1,1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
		shader->setUniform("u_texture", texture);
		shader->setUniform("u_time", time);
		shader->setUniform("u_model", m);

		//draw with the shader
		mesh->render(GL_TRIANGLES, shader);

		//disable shader
		shader->disable();
	}
   
	//Draw out world
	drawGrid();

	//render the FPS
	drawText( 2, 2, "FPS: " + std::to_string(fps) + " DCS: " + std::to_string(Mesh::num_meshes_rendered), fps > 30 ? Vector3(1,1,1) : Vector3(1, 0.5, 0.5), 2 );
	Mesh::num_meshes_rendered = 0;

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around
	if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);

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
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

Mesh* grid = NULL;

void Game::drawGrid()
{
	if (!grid)
	{
		grid = new Mesh();
		grid->createGrid(10);
	}

	glEnable(GL_BLEND);
	glDepthMask(false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Shader* grid_shader = Shader::getDefaultShader("grid");
	grid_shader->enable();
	Matrix44 m;
	m.traslate( floor(camera->eye.x/100.0)*100.0, 0, floor(camera->eye.z / 100.0)*100.0);
	grid_shader->setUniform("u_color", Vector4(0.7, 0.7, 0.7, 0.7));
	grid_shader->setUniform("u_model", m);
	grid_shader->setUniform("u_camera_position", camera->eye );
	grid_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	grid->render(GL_LINES, grid_shader); //background grid
	glDisable(GL_BLEND);
	glDepthMask(true);
	grid_shader->disable();
}