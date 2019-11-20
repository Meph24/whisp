/*
 * Hittable.h
 *
 *  Created on:	04.07.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_HITTABLE_H_
#define SRC_HITTABLE_H_

#include "hitType.h"
#include "ProjectileCollision.h"
#include "DualPointer.h"

class HittableBulletLike;
class Projectile;
class ChunkManager;
class TickServiceProvider;

#include <vector>

class Hittable
{
public:
	hitType acceptedConversions=0;//can have multiple flags set

	virtual void testHit(std::vector<ProjectileCollision> * collisions,hitType type,DualPointer<Projectile> projectile,ChunkManager * cm);
	//void registerHitCheck(Entity * e,float seconds,TickServiceProvider * tsp);

	//below are conversions to more concrete forms of Hittables, subclasses override the correct one and return themselves
	virtual HittableBulletLike * asHittableBulletLike();//default is return null, if not overwritten


	virtual ~Hittable();
};

#endif /* SRC_HITTABLE_H_ */
