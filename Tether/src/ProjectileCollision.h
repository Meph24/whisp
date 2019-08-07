/*
 * ProjectileCollision.h
 *
 *  Created on:	09.07.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_PROJECTILECOLLISION_H_
#define SRC_PROJECTILECOLLISION_H_

#include "DualPointer.h"
#include "Spacevec.h"
#include "hitType.h"

class Hittable;
class Entity;
class HitData;

class ProjectileCollision
{
public:
	float location;//starting point of projectile this tick=0, end point=1
	DualPointer<Hittable> hitVictim;
	hitType type;//TODO evaluate if necessary
	HitData * data=0;//implementation-specific data, can be used or left 0

	spacevec originChunk;//TODO remove after debugging


	bool operator<(const ProjectileCollision& other) const;

	ProjectileCollision(float Location,Entity * asEntity,Hittable * asHittable,hitType myType,spacevec OriginChunk);
	ProjectileCollision(float Location,DualPointer<Hittable> HitVictim,hitType myType);
	~ProjectileCollision();
};
#include "Hittable.h"
#include "Entity.h"
#include "HitData.h"

#endif /* SRC_PROJECTILECOLLISION_H_ */
