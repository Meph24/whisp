/*
 * Item.h
 *
 *  Created on:	02.04.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ITEM_H_
#define SRC_ITEM_H_
#include "ID.h"
#include <cstdlib>

typedef float mass;

class Item
{
public:
	ID UID;
	mass totalMass;
	virtual void tick(Item * future,ItemManager * manager,float sec)=0;
	virtual size_t getSize()=0;
	virtual Item * copyTo(void * memory)=0;//may need certain alignment!

	Item();
	virtual ~Item();
};

#endif /* SRC_ITEM_H_ */
