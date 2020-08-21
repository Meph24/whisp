/*
 * ItemDummy.cpp
 *
 *  Created on:	Jun 14, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemDummy.h"

ItemDummy::ItemDummy(std::string dummyName)
{
	name=dummyName;
	ID.group=ITEM_GROUP_USELESS;
	ID.insideID=0;
	amount=1;
	mass=0.5f;
	volume=0.5f;

}

ItemDummy::~ItemDummy()
{
	// TODO Auto-generated destructor stub
}

void ItemDummy::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
}
