#ifndef FACTORY_H
#define FACTORY_H

#include "Airplane.h"
#include "../framework.h"

class Factory {

	public:

		static Airplane* buildAirplane(const Vector3 initialPos, float speed);

};

#endif // !FACTORY_H
