/*
 * Item.h
 *
 *  Created on:	02.04.2017
 *      Author:	HL65536
 *     Version:	2.0
 */
/*
#ifndef SRC_ITEM_H_
#define SRC_ITEM_H_
#include "ID.h"
#include <cstdlib>

typedef float mass;

class ItemManager;

typedef struct
{
	unsigned short size;//total size including this struct; 0 means invalid, 1 means size was too big and more size bytes are following
	unsigned short type;//65536 different types possible

	//optional: only valid if size==1
	size_t biggerSize;
} compressedInfo;

class Item
{
public:
	ID UID;
	mass totalMass;
	virtual void tick(Item * future,ItemManager * manager,float sec)=0;
	virtual size_t getSize()=0;

	//given memory may need certain alignment!
	virtual Item * copyTo(void * memory)=0;

	//compresses object data and writes it into the given memory; returns actual used memory
	virtual size_t compress(void * memory)=0;

	virtual size_t getMaxCompressedSize()=0;



	Item();
	virtual ~Item();
};

Item * generateItem(void * targetMemory,void * sourceCompressedMemory);

#endif /* SRC_ITEM_H_ */
