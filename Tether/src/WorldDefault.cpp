/*
 * WorldDefault.cpp
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "WorldDefault.h"

#include "Entity.h"
#include "DrawServiceProvider.h"

WorldDefault::WorldDefault(float GridSize):
IWorld(GridSize)
{
}

WorldDefault::~WorldDefault()
{
	for(auto ptr: managedEntities) delete ptr;
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

void WorldDefault::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	for(Entity * e: managedEntities)
	{
		e->tick(next_tick_begin, tsp);
	}
}

void WorldDefault::postTick(TickServiceProvider& tsp)
{
	auto size=managedEntities.size();
	for(auto i=size*0;i<size;i++)
	{
		auto size2=deleteVec.size();
		for(auto j=size2*0;j<size2;j++)
		{
			if(deleteVec[j]==managedEntities[i])
			{
				delete managedEntities[i];
				managedEntities[i]=managedEntities[size-1];
				managedEntities.pop_back();
				deleteVec[j]=deleteVec[size2-1];
				deleteVec.pop_back();
				i--;
				break;
			}
		}
		size=managedEntities.size();
	}
}

void WorldDefault::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum, IWorld& iw,DrawServiceProvider* dsp)
{
	for(Entity * e: managedEntities)
	{
		e->draw(draw_time, viewFrustum,iw,dsp);
		if(dsp->enable_aabbs)
		{
			glColor3f(e->bbColor.r,e->bbColor.g,e->bbColor.b);
			e->bb.draw(draw_time, viewFrustum,iw,dsp);
		}
	}
}

