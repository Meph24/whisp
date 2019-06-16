/*
 * ItemContainer.h
 *
 *  Created on:	Jan 10, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMCONTAINER_H_
#define SRC_ITEMCONTAINER_H_

#include "Item.h"
#include <vector>

class ItemContainer: public Item
{
protected:
	std::vector<Item *> items;
	void insertR(Item * it);

public:
	virtual u32 maximumAdd(Item * item);
	virtual Item * addItem(Item * item);
	virtual Item * removeNextItem(u32 maxCount);//returns the last item with up to an amount of maxCount

	virtual Item * newClone();
	ItemContainer();
	virtual ~ItemContainer();
};

#endif /* SRC_ITEMCONTAINER_H_ */
