/*
 * ItemIdent.cpp
 *
 *  Created on:	Jan 9, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemIdent.h"

ItemIdent::ItemIdent()
{
	// TODO Auto-generated constructor stub

}

ItemIdent::~ItemIdent()
{}

bool ItemIdent::isUnique()
{
	return group==ITEM_GROUP_UNIQUE_TICK && group==ITEM_GROUP_UNIQUE_NOTICK;
}

bool ItemIdent::needsTick()
{
	return group==ITEM_GROUP_UNIQUE_TICK;
}

bool ItemIdent::isInGroup(unsigned int groupID)
{
	return group==groupID;
}

bool ItemIdent::operator ==(ItemIdent other)
{
	return (group==other.group) && (insideID==other.insideID);
}
