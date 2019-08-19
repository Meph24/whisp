/*
 * Projectile.h
 *
 *  Created on:	04.07.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_PROJECTILE_H_
#define SRC_PROJECTILE_H_

#include "Retickable.h"

#include "Spacevec.h"
#include "hitType.h"
#include "DualPointer.h"

class Hittable;
class Entity;
class TickServiceProvider;
class ProjectileCollision;
class EntityProjectileBulletLike;


#include <vector>

class Projectile: public Retickable
{
	std::vector<ProjectileCollision> collisions;
public:
	hitType typeH=0;//must have exactly 1 bit set (by implementations)

	spacevec posOld;
	void interact(Entity * self,DualPointer<Hittable> other, float time, TickServiceProvider* tsp);

	void registerHitCheck(Entity * e,float seconds,TickServiceProvider * tsp);

	virtual void retick(TickServiceProvider * tsp);

	virtual EntityProjectileBulletLike * asProjectileBulletLike();//default is return null, if not overwritten


	//TODO delete collisions at end of retick

	virtual ~Projectile();
};

#endif /* SRC_PROJECTILE_H_ */
