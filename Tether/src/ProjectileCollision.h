/*
 * ProjectileCollision.h
 *
 *  Created on:	09.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_PROJECTILECOLLISION_H_
#define SRC_PROJECTILECOLLISION_H_

#include "DualPointer.h"
#include "Spacevec.h"

class Hittable;
class Entity;

class ProjectileCollision
{
public:
	float location;//starting point of projectile this tick=0, end point=1
	DualPointer<Hittable> hitVictim;
	spacevec originChunk;//TODO remove after debugging


	bool operator<(const ProjectileCollision& other) const;

	ProjectileCollision(float Location,Entity * asEntity,Hittable * asHittable,spacevec OriginChunk);
	ProjectileCollision(float Location,DualPointer<Hittable> HitVictim);
	~ProjectileCollision();
};
#include "Hittable.h"
#include "Entity.h"

#endif /* SRC_PROJECTILECOLLISION_H_ */
