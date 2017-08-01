/*
 * ItemManager.h
 *
 *  Created on:	Apr 6, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */
/*
#ifndef SRC_ITEMMANAGER_H_
#define SRC_ITEMMANAGER_H_

#include "Item.h"
#include "ID.h"

#include <new>

typedef struct
{
	//TODO
	ID UID;

} itemID;

itemID invalidID();

typedef struct
{
	Item * now;
	Item * future;
} itemPtr;


class ItemManager
{

	Item ** itemsNow;
	Item ** itemsFuture;

public:

	itemID insert(Item * item);
	void tick(float seconds);
	void markDelete();
	itemPtr getItem(itemID id);
	void advanceTick();
	void markDelete(itemID item);

	ItemManager();
	~ItemManager();
};

#endif /* SRC_ITEMMANAGER_H_ */
