/*
 * Entity.cpp
 *
 *  Created on:	18.01.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "Entity.h"
#include "ChunkManager.h"
#include "TickServiceProvider.h"

Entity::Entity()
{
	bbColor={1,1,1};
}

void Entity::requestDestroy(IWorld * w)
{
	if(requestedDelete) return;
	requestedDelete=true;
//	exists=false;
	int size=follower.size();
	for(int i=0;i<size;i++)
	{
		follower[i]->notifyRemoval(this);
	}
	follower.clear();
	w->requestEntityDelete(this);
}


//void Entity::prepareForPartitionAlgo(void * filterAlgo,bool multichunkInitValue)
//{
//	if(alreadyChecked.size()) alreadyChecked.clear();
//	multichunk=multichunkInitValue;
//}
//
//bool Entity::hasCheckedAlready(Entity* other, void* filterAlgo)
//{
//}
//
//void Entity::registerAlreadyChecked(Entity* other, void* filterAlgo)
//{
//}
//
//unsigned int Entity::getUselessChecksNumber(void* filterAlgo)
//{
//}
//
//void Entity::ignoreAlreadyChecked(IgnoreCondition* condition, void* filterAlgo)
//{
//	//if(condition->evaluateCondition(e))
//}

#include "WarnErrReporter.h"

void Entity::onLeaveWorld(TickServiceProvider* tsp)
{
	//default: do nothing.
}

void Entity::follow(Entity* e)
{
	follower.push_back(e);
}

void Entity::unfollow(Entity* e)
{
	int found=-1;
	int size=follower.size();
	for(int i=0;i<size;i++)
	{
		if(follower[i]==e)
		{
			found=i;
			break;
		}
	}
	if(found==-1)
	{
		WarnErrReporter::notFoundErr("could not unfollow: not found in followers");
		return;
	}
	follower[found]=follower[size-1];
	follower.pop_back();
}

void Entity::notifyRemoval(Entity* e)
{
	WarnErrReporter::noOverrideErr("Someone requested notifyRemoval and did not override the method");
}


Entity::~Entity()
{
	int size=follower.size();
	if(size>0)
	{
		WarnErrReporter::alreadyDeadErr("someone followed a dead entity");
	}
	for(int i=0;i<size;i++)
	{
		follower[i]->notifyRemoval(this);
	}
}

void Entity::onSpawn(TickServiceProvider* tsp) {}
