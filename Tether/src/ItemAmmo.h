/*
 * ItemAmmo.h
 *
 *  Created on:	Jan 16, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMAMMO_H_
#define SRC_ITEMAMMO_H_

#include "Item.h"
#define ITEM_GROUP_AMMO (ITEM_GROUP_FIRST_FREE+1)

#include "Compatibility.h"

class ItemAmmo: public Item
{
	float bulletMass;//per bullet
	int bulletQuantity;
	float damagePerJoule;//soft tissue damage
	float penResist;//Energy deposition multiplier
	float drag;//calulated from area,Cd,mass,constants
	float explEnergy;//energy contained in the cartridge (0 for crossbow bolt)
	float randomEnergyDev;//as part of whole e.g. 0.02
public:
	Compatibility upC;
	virtual Item * newClone();
	virtual void tick(float time,TickServiceProvider * tsp);
	ItemAmmo();
	virtual ~ItemAmmo();
};

#endif /* SRC_ITEMAMMO_H_ */
