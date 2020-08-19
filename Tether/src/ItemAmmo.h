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
#include "BulletLikeType.h"

class ItemAmmo: public Item
{
public:
	BulletLikeType bulletData;//per bullet
	int bulletQuantity;
	float explEnergy;//energy contained in the cartridge (0 for crossbow bolt)
	float randomEnergyDev;//as part of whole e.g. 0.02
	float spinPenalty;//radius of barrel minus bullet radius; unit: m
	Compatibility containerC;
	Compatibility gunC;
	virtual Item * newClone();
	virtual void tick(const SimClock::time_point& next_tick_begin,TickServiceProvider * tsp);
	ItemAmmo(float spd,float dmg,float drg,int bulletCount);//TODO new constructor when used together with guns 2.0
	virtual ~ItemAmmo();
};

#endif /* SRC_ITEMAMMO_H_ */
