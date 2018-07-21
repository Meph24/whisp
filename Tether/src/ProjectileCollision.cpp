/*
 * ProjectileCollision.cpp
 *
 *  Created on:	09.07.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ProjectileCollision.h"


ProjectileCollision::ProjectileCollision(float Location, Entity* asEntity,Hittable* asHittable,spacevec OriginChunk):
location(Location),hitVictim(asEntity,asHittable),originChunk(OriginChunk)
{
}

ProjectileCollision::ProjectileCollision(float Location,DualPointer<Hittable> HitVictim):
location(Location),hitVictim(HitVictim)
{
}

bool ProjectileCollision::operator <(const ProjectileCollision& other) const
{
	return location<other.location;
}

ProjectileCollision::~ProjectileCollision()
{}

