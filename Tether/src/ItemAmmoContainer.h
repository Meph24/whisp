/*
 * ItemAmmoContainer.h
 *
 *  Created on:	Apr 12, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */
/*
#ifndef SRC_ITEMAMMOCONTAINER_H_
#define SRC_ITEMAMMOCONTAINER_H_

#include "ItemManager.h"

class ItemAmmoContainer: public Item
{
public:
	ItemAmmoContainer();
	~ItemAmmoContainer();

	unsigned int capacity;
	unsigned int type;//bit 0: fifo; bit 1: access everything allowed
	itemID * content;

	unsigned int next;
	unsigned int last;


	//return if successful
	bool insert(itemID item);
	//return if successful
	bool insertAt(unsigned int pos,itemID item);

	//return if successful
	bool advance();

	//returns invalid if item could not be pulled (for whatever reason)
	itemID pullOut();
	//returns invalid if item could not be pulled (for whatever reason)
	itemID pullOutAt(unsigned int pos);

	void tick(Item * future,ItemManager * manager,float sec);
	size_t getSize();
	Item * copyTo(void * memory);
};

#endif /* SRC_ITEMAMMOCONTAINER_H_ */
