/*
 * Zombie_ItemGun.h
 *
 *  Created on:	12.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ZOMBIE_ITEMGUN_H_
#define SRC_ZOMBIE_ITEMGUN_H_

#include "Zombie_GunType.h"

#define GUN_READY 0
#define GUN_LOCKED 1
#define GUN_UNLOCKING 2
#define GUN_FIRING 3
#define GUN_LOADING 4

class Zombie_ItemGun
{
public:
	int typeID;
	Zombie_GunType * type;//must always match typeID
	int primaryState;
	float primaryCooldown;

	Zombie_ItemGun();
	~Zombie_ItemGun();
	void tick(float time);//sec
	int fire();//returns -1 if nothing fired, ammo ID otherwise
	void unlock();
	void loadManually();

};

#endif /* SRC_ZOMBIE_ITEMGUN_H_ */
