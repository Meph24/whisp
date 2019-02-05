/*
 * Projectile.h
 *
 *  Created on:	04.07.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_PROJECTILE_H_
#define SRC_PROJECTILE_H_

#include <vector>
#include "Spacevec.h"
#include "hitType.h"
#include "Hittable.h"
#include "DualPointer.h"
#include "Retickable.h"
class Entity;
class TickServiceProvider;
class ProjectileCollision;
class ProjectileBulletLike;
class Projectile: public Retickable
{
	std::vector<ProjectileCollision> collisions;
public:
	hitType type=0;//must have exactly 1 bit set (by implementations)

	spacevec posOld;
	void interact(Entity * self,DualPointer<Hittable> other, float time, TickServiceProvider* tsp);

	void registerHitCheck(Entity * e,float seconds,TickServiceProvider * tsp);

	virtual void retick(TickServiceProvider * tsp);

	virtual ProjectileBulletLike * asProjectileBulletLike();//default is return null, if not overwritten


	//TODO delete collisions at end of retick

	virtual ~Projectile();
};
#include "Entity.h"
#include "TickServiceProvider.h"
#include "ProjectileCollision.h"
#include "ProjectileBulletLike.h"

#endif /* SRC_PROJECTILE_H_ */
