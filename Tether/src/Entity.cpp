/*
 * Entity.cpp
 *
 *  Created on:	18.01.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "Entity.h"

Entity::Entity()
{}

void Entity::requestDestroy(TickServiceProvider* tsp)
{
	exists=false;
	tsp->requestDestroy(this);
}

//bool Entity::doAABBcheck(Entity* other, float time,TickServiceProvider* tsp)
//{
//	if(tsp->tickID!=lastTickID)
//	{
//		reset();
//		lastTickID=tsp->tickID;
//	}
//	if(!bb.doesIntersect(other->bb)) return false;
//	AABB::intersectionCounter++;
//	if(multichunk&&other->multichunk)
//	{
//		int size=alreadyChecked.size();
//		for(int i=0;i<size;i++)
//		{
//			if(other==alreadyChecked[i]) return false;
//		}
//		alreadyChecked.push_back(other);
//		other->alreadyChecked.push_back(this);
//	}
//	return true;
//}

void Entity::reset()
{
	if(multichunk) alreadyChecked.clear();
	multichunk=bb.isMultichunk();
}

#include "WarnErrReporter.h"



Entity::~Entity()
{
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
