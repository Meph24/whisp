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
{}

void Entity::requestDestroy(IWorld * w)
{
	if(requestedDelete) return;
	requestedDelete=true;
	exists=false;
	int size=follower.size();
	for(int i=0;i<size;i++)
	{
		follower[i]->notifyRemoval(this);
	}
	follower.clear();
	w->requestEntityDelete(this);
}


void Entity::reset()
{
	if(multichunk) alreadyChecked.clear();
	multichunk=bb.isMultichunk();
}

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

/*
void Entity::standardMove(float time,ChunkManager * cm)
{
	if(isInAir)
	{
		v.y-=cm->getGravity()*time;//apply gravity
	}
	else if(selfMovement)
	{
		//apply wanted movement;
	}
	pos+=v*time;
	//apply movement


	//TODO
	//move according to movement vector, wanted movement, gravity
	//does not apply custom forces like air friction
}

void Entity::move(float time, ChunkManager* cm)
{
	standardMove(time,cm);
}
*/
