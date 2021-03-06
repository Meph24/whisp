/*
 * ItemContainer.h
 *
 *  Created on:	Jan 10, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMCONTAINER_H_
#define SRC_ITEMCONTAINER_H_

#include <vector>

#include "Item.h"

#include "SimulationInputStatusSet.hpp"

class ItemContainer: public Item
{
	unsigned int selected=0;//item slot selected
	unsigned int firstInList=0;//item on top of scrollable list

	i64 consumable_select_add = 0;

protected:
	const u32 maxListLen=32;

	void insertR(Item * it);

public:
	std::vector<Item *> items;
	virtual u32 maximumAdd(Item * item);
	virtual Item * addItem(Item * item);
	virtual Item * removeNextItem(u32 maxCount);//returns the last item with up to an amount of maxCount
	
	void selectRelative( i64 select_add );

	virtual Item * newClone();
	virtual void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw,Perspective& perspective);//when held by player: must draw GUI and/or the physical item
	virtual void tick(const SimClock::time_point& next_tick_begin, TickServiceProvider * tsp);

	ItemContainer();
	virtual ~ItemContainer();
};

#endif /* SRC_ITEMCONTAINER_H_ */
