#ifndef MACHINE_GUN_H
#define MACHINE_GUN_H

#include "Weapon.h"

class Airplane;

class MachineGun : public Weapon {

	public:
		
		MachineGun(Airplane* owner, std::string type);

		virtual void shoot();
		virtual void update(float deltaTime);
};


#endif