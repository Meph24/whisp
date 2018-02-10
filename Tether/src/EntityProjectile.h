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
	ItemAmmo * fromItem;
	vec3 posOld;




	static ITexture * tex;
public:
	EntityProjectile(ItemAmmo * item,vec3 position,vec3 velocity);//gives item ownership to the Projectile
	~EntityProjectile();
	virtual void draw();
	virtual void tick(float time,TickServiceProvider * tsp);

	static void setTexture(ITexture * texture);
};

ITexture * EntityProjectile::tex=new TextureDummy();

#endif /* SRC_ENTITYPROJECTILE_H_ */
