/*
 * ItemAmmo.cpp
 *
 *  Created on:	Jan 16, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemAmmo.h"

ItemAmmo::ItemAmmo()
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
