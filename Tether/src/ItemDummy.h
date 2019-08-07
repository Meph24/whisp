/*
 * ItemDummy.h
 *
 *  Created on:	Jun 14, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMDUMMY_H_
#define SRC_ITEMDUMMY_H_

#include "Item.h"

class ItemDummy: public Item
{
public:
	ItemDummy(std::string dummyName="Duftkerze");
	~ItemDummy();
	virtual void tick(Timestamp t,TickServiceProvider * tsp);
};

#endif /* SRC_ITEMDUMMY_H_ */
