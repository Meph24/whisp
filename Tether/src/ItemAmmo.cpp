/*
 * ItemAmmo.cpp
 *
 *  Created on:	Jan 16, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemAmmo.h"

ItemAmmo::ItemAmmo(float spd,float dmg,float drg, int bulletCount):
bulletData(0.001f,dmg,dmg,drg),bulletQuantity(bulletCount),explEnergy(spd)
{
	ID.group=ITEM_GROUP_AMMO;
}

ItemAmmo::~ItemAmmo()
{
	// TODO Auto-generated destructor stub
}

Item* ItemAmmo::newClone()
{
	return new ItemAmmo(*this);
}

void ItemAmmo::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp)
{}
