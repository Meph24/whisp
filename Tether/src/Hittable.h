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
#include "BulletHittable.h"
class ProjectileCollision;
#include "ProjectileCollision.h"
#include "DualPointer.h"
class Projectile;
class Hittable
{
public:
	hitType acceptedConversions=0;

	virtual void testHit(std::vector<ProjectileCollision> * collisions,DualPointer<Projectile> projectile,ChunkManager * cm);
	void registerHitCheck(Entity * e,float seconds,TickServiceProvider * tsp);
	virtual BulletHittable * asBulletHittable();//default is return null, if not overwritten
	virtual ~Hittable();
};

#include "Projectile.h"

#endif /* SRC_HITTABLE_H_ */
