#ifndef MACHINE_GUN_H
#define MACHINE_GUN_H

#include "Weapon.h"

class Airplane;

class MachineGun : public Weapon {

	public:
		
		MachineGun(Airplane* owner, char type);

		virtual void shoot();
};


#endif