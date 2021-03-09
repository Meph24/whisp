/*
 * WorldDefault.cpp
 *
 *  Created on:	Nov 20, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "WorldDefault.h"

#include "Entity.h"
#include "Perspective.hpp"

WorldDefault::WorldDefault(float GridSize)
	: IWorld(GridSize)
{}

WorldDefault::~WorldDefault()
{
	assert(entityNotif.registered.empty());
	for(auto ptr: managedEntities) delete ptr;
}


void WorldDefault::clearEntities()
{
	std::vector<Entity *> remain;
	for(Entity * e: managedEntities)
	{
		if(e->surviveClearing)
		{
			remain.push_back(e);
		}
		else
		{
			entityNotif.notifyDestruction(e);
			delete e;
		}
	}
	managedEntities.clear();

	for(Entity * e: remain)
	{
		managedEntities.push_back(e);
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

#include "vectorTools.hpp"
void WorldDefault::postTick(TickServiceProvider& tsp)
{
	auto size=managedEntities.size();
	for(auto i=size*0;i<size;i++)//TODO implement this algorithm into vectorTools
	{
		auto size2=deleteVec.size();
		for(auto j=size2*0;j<size2;j++)
		{
			if(deleteVec[j]==managedEntities[i])
			{
				Entity * toDelete=managedEntities[i];
				removeIndexUnordered(managedEntities,i);
				removeIndexUnordered(deleteVec,j);
				entityNotif.notifyDestruction(toDelete);
				delete toDelete;
				i--;
				break;
			}
		}
		size=managedEntities.size();
	}
	for(Entity * e: addVec)
	{
		managedEntities.push_back(e);
		entityNotif.notifyCreation(e);
	}
	addVec.clear();
	{
		std::lock_guard lock(tsAddVecMutex);
		for(Entity * e: tsAddVec)
		{
			managedEntities.push_back(e);
			entityNotif.notifyCreation(e);
		}
		tsAddVec.clear();
	}
}

void WorldDefault::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum, IWorld& iw, Perspective& perspective)
{
	for(Entity * e: managedEntities)
	{
		e->draw(draw_time, viewFrustum,iw,perspective);
		if(perspective.enable_aabbs)
		{
			glColor3f(e->bbColor.r,e->bbColor.g,e->bbColor.b);
			e->bb.draw(draw_time, viewFrustum,iw,perspective);
		}
	}
}

void WorldDefault::notifyCreation(Entity* obj)
{
	managedEntities.push_back(obj);
}

void WorldDefault::notifyDestruction(Entity* obj)
{
	removeElementUnordered(managedEntities,obj);
	obj->destroyCleanup();
}

void WorldDefault::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	for(auto e : managedEntities)
		collectHere.push_back(e);
}
