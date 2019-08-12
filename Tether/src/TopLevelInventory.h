/*
 * TopLevelInventory.h
 *
 *  Created on:	25.07.2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_TOPLEVELINVENTORY_H_
#define SRC_TOPLEVELINVENTORY_H_

#include "ItemContainer.h"
class EntityPlayer;

class TopLevelInventory: public ItemContainer
{
	EntityPlayer * owner;
public:
	TopLevelInventory(EntityPlayer * myOwner);
	virtual ~TopLevelInventory();
};

#include "EntityPlayer.h"
#endif /* SRC_TOPLEVELINVENTORY_H_ */
