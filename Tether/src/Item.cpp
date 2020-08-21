/*
 * Item.cpp
 *
 *  Created on:	02.04.2017
 *      Author:	HL65536
 *     Version:	2.0.1
 */

#include "Item.h"

u32 Item::notickIDcounter=1;
u32 Item::tickIDcounter=1;

Item::Item()
{
	// TODO Auto-generated constructor stub

}


Item* Item::newClone()
{
	return 0;
}

void Item::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum,IWorld& iw,DrawServiceProvider* dsp)
{}

void Item::giveNewUniqueNotickID()
{
	ID.group=ITEM_GROUP_UNIQUE_NOTICK;
	ID.insideID=notickIDcounter;
	notickIDcounter++;
}

void Item::giveNewUniqueTickID()
{
	ID.group=ITEM_GROUP_UNIQUE_TICK;
	ID.insideID=tickIDcounter;
	tickIDcounter++;
}

Item::~Item()
{
	// TODO Auto-generated destructor stub
}

float Item::getTotalVolume()
{
	return amount*volume;
}

float Item::getTotalMass()
{
	return amount*mass;
}

std::string Item::getDisplayString()
{
	std::string ret=name;
	if(ID.group==ITEM_GROUP_UNIQUE_TICK||ID.group==ITEM_GROUP_UNIQUE_NOTICK) ret="- "+ret;
	else ret=std::to_string(amount)+ret;
	return ret;
}
