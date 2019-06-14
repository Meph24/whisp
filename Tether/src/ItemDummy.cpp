/*
 * ItemDummy.cpp
 *
 *  Created on:	Jun 14, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemDummy.h"

ItemDummy::ItemDummy()
{
	name="Duftkerze";
	ID.group=ITEM_GROUP_USELESS;
	ID.insideID=0;
	mass=0.5f;
	volume=0.5f;

}

ItemDummy::~ItemDummy()
{
	// TODO Auto-generated destructor stub
}

