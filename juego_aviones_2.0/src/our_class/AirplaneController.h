#ifndef AIRPLANE_CONTROLLER_H
#define AIRPLNE_CONTROLLER_H

class Entity;
class Airplane;

class AirplaneController {
	public:

		Airplane* airplane;

		AirplaneController();
		~AirplaneController();

		virtual void update();

};

#endif
