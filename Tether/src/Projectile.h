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
#include "Hittable.h"
#include "DualPointer.h"
#include "Retickable.h"
class Entity;
class TickServiceProvider;
class ProjectileCollision;
class Projectile: public Retickable
{
	std::vector<ProjectileCollision> collisions;
public:
	hitType type=0;

	spacevec posOld;
	void interact(Entity * self,DualPointer<Hittable> other, float time, TickServiceProvider* tsp);

	void registerHitCheck(Entity * e,float seconds,TickServiceProvider * tsp);

	virtual void retick(TickServiceProvider * tsp);

	//TODO delete collisions at end of retick

	virtual ~Projectile();
};
#include "Entity.h"
#include "TickServiceProvider.h"
#include "ProjectileCollision.h"

#endif /* SRC_PROJECTILE_H_ */
