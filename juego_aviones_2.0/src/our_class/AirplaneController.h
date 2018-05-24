#ifndef AIRPLANE_CONTROLLER_H
#define AIRPLNE_CONTROLLER_H

class Entity;
class Airplane;

class AirplaneController {
	public:

		Airplane* airplane;

		AirplaneController();
		~AirplaneController();

		bool canSeeEntity(Entity* entity);
		bool haveAnyEntityNear(Entity* entity);
		void render();
		virtual void update();

};

#endif
