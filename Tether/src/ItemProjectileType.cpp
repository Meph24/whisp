/*
 * ItemProjectileType.cpp
 *
 *  Created on:	Apr 6, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "ItemProjectileType.h"

ItemProjectileType::ItemProjectileType()
{
	// TODO Auto-generated constructor stub

}

ItemProjectileType::~ItemProjectileType()
{
	// TODO Auto-generated destructor stub
}


void ItemProjectileType::tick(Item * future,ItemManager * manager,float sec)
{
	//TODO
}

size_t ItemProjectileType::getSize()
{
	return sizeof(*this);
}

Item * ItemProjectileType::copyTo(void * memory)
{
	return (Item *) new (memory) ItemProjectileType(*this);
}
