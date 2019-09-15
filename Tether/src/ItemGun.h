/*
 * ItemGun.h
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */


/*
#ifndef SRC_ITEMGUN_H_
#define SRC_ITEMGUN_H_

#include "Item.h"

class ItemAmmo;

#include "vec3.h"
#include "RecoilReturning.h"
#include "Compatibility.h"

class ItemGun: public Item
{
	float barrelEfficiency;//longer barrels are more efficient; 0-1
	float additionalEnergy;//used in crossbow to store energy; unit: J

	float rotation;//twist of the barrel,
	//influences spread +/- depending on type of ammo,
	//unit: rad/m

	ItemAmmo * chamber;
	Compatibility ammoC;
	Compatibility containerC;


	vec3 recoil;
	vec3 recoilSpread;
	RecoilReturning recoilM;

	bool triggerIsPulled;

public:
	void fire(TickServiceProvider * tsp);//this is called when the logic decided that a shot should fall, uses ammo in chamber and sets it to 0

	void loadChamber(ItemAmmo * ammo);//this is called when the logic decided that now a shot is being loaded into the chamber

	void trigger(bool pulled);//call this whenever triggerIsPulled status should change

	virtual void tick(Timestamp t,TickServiceProvider * tsp);
	ItemGun();
	virtual ~ItemGun();
};

#endif /* SRC_ITEMGUN_H_ */

*/
