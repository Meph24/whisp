/*
 * Zombie_AmmoType.h
 *
 *  Created on:	12.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ZOMBIE_AMMOTYPE_H_
#define SRC_ZOMBIE_AMMOTYPE_H_
#include "Compatibility.h"
class Zombie_AmmoType
{
public:
	int ID;
	Compatibility comp;
	float speed;
	float mass;//kg
	float drag;//area*Cd/mass*0.5*1.293
	float penResist;//multiply with usually spent energy while penetrating to get the needed energy with this projectile
	float damage;//multiply with spent energy to get total damage value
	Zombie_AmmoType(float spd,float dmg,float drg);
	~Zombie_AmmoType();
};

#endif /* SRC_ZOMBIE_AMMOTYPE_H_ */
