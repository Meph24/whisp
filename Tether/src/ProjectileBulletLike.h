/*
 * ProjectileBulletLike.h
 *
 *  Created on:	31.01.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_PROJECTILEBULLETLIKE_H_
#define SRC_PROJECTILEBULLETLIKE_H_

#include "HittableBulletLike.h"
#include "ProjectileCollision.h"
#include "Projectile.h"
#include "DualPointer.h"

class ProjectileBulletLike
{
public:
	float deformation=0;//0-1


	ProjectileBulletLike();
	~ProjectileBulletLike();

	bool collide(DualPointer<Projectile> self,HittableBulletLike * hittable,ProjectileCollision collision,TickServiceProvider* tsp);//return if projectile continues to fly
};

#endif /* SRC_PROJECTILEBULLETLIKE_H_ */
