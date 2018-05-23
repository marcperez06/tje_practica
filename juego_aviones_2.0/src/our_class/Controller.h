#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
	public:

		bool canSeeEntity(Entity* entity);

		virtual void update();

};

#endif
