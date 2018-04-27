#ifndef BUILD_FACTORY_H
#define BUILD_FACTORY_H

#include "../framework.h"

class BuildFactory {

	public:

		static Airplane* buildPlayer(Vector3 const initialPos);

};

#endif // !BUILD_FACTORY_H
