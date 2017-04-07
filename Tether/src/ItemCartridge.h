/*
 * ItemCartridge.h
 *
 *  Created on:	Apr 6, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ITEMCARTRIDGE_H_
#define SRC_ITEMCARTRIDGE_H_

#include "ItemManager.h"
#include "ItemProjectileType.h"

class ItemCartridge:
		public Item
{
public:
	mass privateMass;

	float energy;//(joules) only includes energy stored in cartridge

	float caliber;//diameter of barrel
	float maxTotalLength;
	float maxProjectileLength;
	unsigned int designLayout;//44 magnum and 44 special have same designLayout, but different than 357 magnum
	bool allowSmallProjectiles;//used for shotgun shells

	unsigned int projectileTypes;
	itemID * types;

	void tick(Item * future,ItemManager * manager,float sec);
	size_t getSize();
	Item * copyTo(void * memory);

	bool fits(ItemProjectileType * p);

	ItemCartridge();
	~ItemCartridge();
};

#endif /* SRC_ITEMCARTRIDGE_H_ */
