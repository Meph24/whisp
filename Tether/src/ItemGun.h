/*
 * ItemGun.h
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ITEMGUN_H_
#define SRC_ITEMGUN_H_

#include "ItemAmmo.h"
#include "MatrixLib.h"
#include "RecoilReturning.h"

class ItemGun: public Item
{
	float barrelEfficiency;//longer barrels are more efficient; 0-1
	float additionalEnergy;//used in crossbow to store energy; unit: J

	float rotation;//twist of the barrel,
	//influences spread +/- depending on type of ammo,
	//unit: rad/m




	vec3 recoil;
	vec3 recoilSpread;
	RecoilReturning recoilM;

public:
	void fire(ItemAmmo * ammo,TickServiceProvider * tsp);//this is called when the logic decided that a shot should fall, does not delete the item

	virtual void tick(float time,TickServiceProvider * tsp);
	ItemGun();
	virtual ~ItemGun();
};

#endif /* SRC_ITEMGUN_H_ */
