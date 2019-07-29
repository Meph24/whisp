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
#include "Graphics2D.h"

class ItemContainer: public Item
{
	static Graphics2D * g;

	unsigned int selected=0;//item slot selected
	unsigned int firstInList=0;//item on top of scrollable list



protected:
	std::vector<Item *> items;
	void insertR(Item * it);

public:
	virtual u32 maximumAdd(Item * item);
	virtual Item * addItem(Item * item);
	virtual Item * removeNextItem(u32 maxCount);//returns the last item with up to an amount of maxCount

	virtual Item * newClone();
	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp);//when held by player: must draw GUI and/or the physical item

	ItemContainer();
	virtual ~ItemContainer();
};

#endif /* SRC_ITEMCONTAINER_H_ */
