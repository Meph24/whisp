/*
 * ItemGun.h
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMGUN_H_
#define SRC_ITEMGUN_H_

#include "Item.h"

class ItemGun: public Item
{
public:
	virtual void tick(float time);
	ItemGun();
	virtual ~ItemGun();
};

#endif /* SRC_ITEMGUN_H_ */
