/*
 * WorldDefault.cpp
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "WorldDefault.h"

#include "Entity.h"

WorldDefault::WorldDefault():
IWorld(16)
{
}

WorldDefault::~WorldDefault()
{
	// TODO Auto-generated destructor stub
}

void WorldDefault::requestEntitySpawn(Entity* e)
{
	managedEntities.push_back(e);
}

void WorldDefault::clearEntities()
{
	std::vector<Entity *> remain;
	int size=managedEntities.size();
	for(int i=0;i<size;i++)
	{
		if(managedEntities[i]->surviveClearing)
		{
			remain.push_back(managedEntities[i]);
		}
		else
		{
			delete managedEntities[i];
		}
	}
	managedEntities.clear();

	size=remain.size();
	for(int i=0;i<size;i++)
	{
		managedEntities.push_back(remain[i]);
	}
	remain.clear();
}

void WorldDefault::tick(Timestamp t, TickServiceProvider* tsp)
{
	for(Entity * e: managedEntities)
	{
		e->tick(t,tsp);
	}
}

void WorldDefault::postTick(TickServiceProvider* tsp)
{
}

void WorldDefault::draw(Timestamp t, Frustum* viewFrustum, IWorld& iw,DrawServiceProvider* dsp)
{
	for(Entity * e: managedEntities)
	{
		e->draw(t,viewFrustum,iw,dsp);
	}
}
