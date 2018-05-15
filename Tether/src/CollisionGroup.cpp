/*
 * CollisionGroup.cpp
 *
 *  Created on:	06.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "CollisionGroup.h"
#include "AABB.h"

CollisionGroup::CollisionGroup()
{}

void CollisionGroup::registerCollisionCheck(Entity* e, float time,TickServiceProvider* tsp)
{
	int size=registered.size();
	for(int i=0;i<size;i++)
	{
		check(e,registered[i],time,tsp);
		AABB::checkCounter++;
	}
	registered.push_back(e);
}

void CollisionGroup::reset()
{
	registered.clear();
}

void CollisionGroup::check(Entity* e, Entity* r, float time,TickServiceProvider* tsp)
{
	e->doAABBcheck(r,time,tsp);
}

CollisionGroup::~CollisionGroup()
{}

