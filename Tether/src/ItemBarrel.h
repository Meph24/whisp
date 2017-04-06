/*
 * ItemBarrel.h
 *
 *  Created on:	03.04.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ITEMBARREL_H_
#define SRC_ITEMBARREL_H_

#include "ItemManager.h"

class ItemBarrel: public Item
{
public:
	mass privateMass;

	float caliber;// = cartridge caliber
	float maxLength;// >= cartridge length
	unsigned int designLayout;//cartridge must share this number in order to fit


	float efficiency;//gunpowder energy applied to projectiles; between 0 and 1

	float riflingValue;//0=no rifling 1=perfect rifling

	float recoilMult;//1=normal recoil; 0=no recoil

	float soundMult;//1=normal sound volume; 0=perfectly silent

	itemID loadedCartridge;



	ItemBarrel();
	~ItemBarrel();

	void tick(Item * future,ItemManager * manager,float sec);
	size_t getSize();
	Item * copyTo(void * memory);

	bool fits(ItemCartridge * c);


	itemID trigger();
};

#endif /* SRC_ITEMBARREL_H_ */
