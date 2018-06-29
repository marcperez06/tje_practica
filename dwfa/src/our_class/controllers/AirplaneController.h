#ifndef AIRPLANE_CONTROLLER_H
#define AIRPLNE_CONTROLLER_H
#pragma once

class Airplane;

class AirplaneController {
	public:

		Airplane* airplane;

		AirplaneController();
		AirplaneController(Airplane* airplane);
		~AirplaneController();

		virtual void render();
		virtual void update(float deltaTime);

};

#endif
