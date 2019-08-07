/*
 * EntityIdent.cpp
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "EntityIdent.h"

EntityIdent::EntityIdent()
{
	// TODO Auto-generated constructor stub

}


EntityIdent::~EntityIdent()
{
	// TODO Auto-generated destructor stub
}


bool EntityIdent::isInGroup(unsigned int groupID)
{
	return group==groupID;
}
