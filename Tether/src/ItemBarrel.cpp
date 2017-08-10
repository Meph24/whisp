/*
 * ItemBarrel.cpp
 *
 *  Created on:	03.04.2017
 *      Author:	HL65536
 *     Version:	1.0
 */
/*
#include "ItemBarrel.h"

ItemBarrel::ItemBarrel()
{
	// TODO Auto-generated constructor stub

}

ItemBarrel::~ItemBarrel()
{
	// TODO Auto-generated destructor stub
}


void ItemBarrel::tick(Item * future,ItemManager * manager, float sec)
{
	//TODO
}


size_t ItemBarrel::getSize()
{
	return sizeof(*this);
}
Item * ItemBarrel::copyTo(void * memory)
{
	return (Item *) new (memory) ItemBarrel(*this);
}

itemID ItemBarrel::trigger()
{
	itemID ret=loadedCartridge;
	loadedCartridge=invalidID();
	return ret;
}

bool ItemBarrel::fits(ItemCartridge * c)
{
	if(c->designLayout==designLayout) return false;
	float c1=c->caliber;
	if(c1>caliber+0.0001f) return false;
	if(c1<caliber-0.0001f) return false;
	if(maxLength+0.0001f<c->maxTotalLength) return false;
	return true;

}
*/
