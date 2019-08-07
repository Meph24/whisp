/*
 * ItemProjectileType.h
 *
 *  Created on:	Apr 6, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */
/*
#ifndef SRC_ITEMPROJECTILETYPE_H_
#define SRC_ITEMPROJECTILETYPE_H_

#include "ItemManager.h"
class ItemProjectileType:
	public Item
{
public:
	unsigned int spawnCount;//number of identical projectiles contained here

	float cwA;//air resistance*area

	float mass;//kilograms per projectile

	float penetration;//higher value=easier penetration

	float tissueDamage;//only a postmultiplier

	float precision;//random error size around most common impact point

	float precisionRiflingScaling;//random error scaling with rifling (positive for bullets, negative for shot)

	float caliber;//meters

	float length;//meters

	void tick(Item * future,ItemManager * manager,float sec);
	size_t getSize();
	Item * copyTo(void * memory);

	ItemProjectileType();
	~ItemProjectileType();
};

#endif /* SRC_ITEMPROJECTILETYPE_H_ */
