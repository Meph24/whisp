/*
 * ItemMagazine.h
 *
 *  Created on:	Jan 15, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMMAGAZINE_H_
#define SRC_ITEMMAGAZINE_H_

#include "ItemContainer.h"

#include "Compatibility.h"

class ItemMagazine: public ItemContainer
{
	int capacity;
	Compatibility downC;
	Compatibility upC;
public:
	virtual u32 maximumAdd(Item * item);
	ItemMagazine();
	virtual ~ItemMagazine();
};

#endif /* SRC_ITEMMAGAZINE_H_ */
