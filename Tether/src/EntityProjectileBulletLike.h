/*
 * EntityProjectile.h
 *
 *  Created on:	Jan 18, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITYPROJECTILEBULLETLIKE_H_
#define SRC_ENTITYPROJECTILEBULLETLIKE_H_

#include "Entity.h"
#include "Projectile.h"

#include "BulletLikeType.h"
#include "ProjectileCollision.h"

class BulletLikeSource;
class ITexture;
class HittableBulletLike;

//#include "ItemAmmo.h"

class EntityProjectileBulletLike: public Entity, public Projectile
{
public:
	float deformation=0;//0-1

	static ITexture * tex;//TODO find better texture loading solution
//	ItemAmmo * fromItem;
	BulletLikeType typeB;

	BulletLikeSource * source;

	EntityProjectileBulletLike(BulletLikeSource * origin,BulletLikeType t,Timestamp spawnTime,spacevec position,spacevec velocity);//gives item ownership to the Projectile
	~EntityProjectileBulletLike();
	virtual void draw(Timestamp t,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp);
	virtual void tick(Timestamp t,TickServiceProvider * tsp);

	bool collide(HittableBulletLike * hittable,ProjectileCollision collision,TickServiceProvider* tsp);//return if projectile continues to fly

	virtual EntityProjectileBulletLike * asProjectileBulletLike();


	static void setTexture(ITexture * texture);
};

#endif /* SRC_ENTITYPROJECTILEBULLETLIKE_H_ */
