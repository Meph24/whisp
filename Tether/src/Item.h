/*
 * Item.h
 *
 *  Created on:	16.01.2018
 *      Author:	HL65536
 *     Version:	2.0.1
 */

#ifndef SRC_ITEM_H_
#define SRC_ITEM_H_
#include "ItemIdent.h"
#include "ShortNames.h"
#include "Tickable.h"
#include <string>
class Item: public Tickable
{
	std::string name;//TODO localized

public:
	ItemIdent ID;
	u32 amount;//non-unique items can be stacks of arbitrary size, stackable items must be copyable
	float mass;//per unit (kg)
	float volume;//per unit (l)

	float getTotalVolume();
	float getTotalMass();
	std::string getDisplayString();

	virtual Item * newClone();

	Item();
	virtual ~Item();
};

/*02.04.2017
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
*/
#endif /* SRC_ITEM_H_ */
