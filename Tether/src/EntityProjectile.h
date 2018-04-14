/*
 * EntityProjectile.h
 *
 *  Created on:	Jan 18, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ENTITYPROJECTILE_H_
#define SRC_ENTITYPROJECTILE_H_
#include "Entity.h"
#include "ItemAmmo.h"
#include "TextureDummy.h"

class EntityProjectile: public Entity
{
	static ITexture * tex;
public:
	ItemAmmo * fromItem;//TODO private?
	spacevec posOld;//TODO private?

	EntityProjectile(ItemAmmo * item,spacevec position,spacevec velocity);//gives item ownership to the Projectile
	~EntityProjectile();
	virtual void draw(float tickOffset,spacevec observerPos,ChunkManager * cm,DrawServiceProvider * dsp);
	virtual void tick(float time,TickServiceProvider * tsp);

	static void setTexture(ITexture * texture);
};


#endif /* SRC_ENTITYPROJECTILE_H_ */
