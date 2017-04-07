/*
 * ItemCartridge.cpp
 *
 *  Created on:	Apr 6, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "ItemCartridge.h"

ItemCartridge::ItemCartridge()
{
	// TODO Auto-generated constructor stub

}

ItemCartridge::~ItemCartridge()
{
	// TODO Auto-generated destructor stub
}



void ItemCartridge::tick(Item * future,ItemManager * manager,float sec)
{
	//TODO
}

size_t ItemCartridge::getSize()
{
	return sizeof(*this);
}

Item * ItemCartridge::copyTo(void * memory)
{
	return (Item *) new (memory) ItemCartridge(*this);
}

