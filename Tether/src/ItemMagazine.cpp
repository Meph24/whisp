/* ItemMagazine.cpp
 *
 *  Created on:	Jan 15, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemMagazine.h"
#include "ItemAmmo.h"
ItemMagazine::ItemMagazine()
{
	ID.group=ITEM_GROUP_UNIQUE_NOTICK;
}

u32 ItemMagazine::maximumAdd(Item* item)
{
	if(item->ID.group!=ITEM_GROUP_AMMO) return 0;
	if(!((ItemAmmo *) item)->containerC.doIFitInside(downC)) return 0;
	int ret=capacity;
	int c=items.size();
	for(int i = 0 ; i<c ; i++)
	{
		ret-=items[i]->amount;
	}
	return ret;
}

ItemMagazine::~ItemMagazine()
{

}

