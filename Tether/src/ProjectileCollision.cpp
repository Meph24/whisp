/*
 * ProjectileCollision.cpp
 *
 *  Created on:	09.07.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "ProjectileCollision.h"
#include "HitData.h"


ProjectileCollision::ProjectileCollision(float Location, Entity* asEntity,Hittable* asHittable,hitType myType,spacevec OriginChunk):
location(Location),hitVictim(asEntity,asHittable),type(myType),originChunk(OriginChunk)
{
}

ProjectileCollision::ProjectileCollision(float Location,DualPointer<Hittable> HitVictim,hitType myType):
location(Location),hitVictim(HitVictim),type(myType)
{
}

bool ProjectileCollision::operator <(const ProjectileCollision& other) const
{
	return location<other.location;
}

ProjectileCollision::~ProjectileCollision()
{
	if(data) delete data;
}

