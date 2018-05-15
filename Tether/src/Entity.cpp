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

void Entity::doAABBcheck(Entity* other, float time,TickServiceProvider* tsp)
{
	if(!bb.doesIntersect(other->bb)) return;
	AABB::intersectionCounter++;
	if(multichunk&&other->multichunk)
	{
		int size=alreadyChecked.size();
		for(int i=0;i<size;i++)
		{
			if(other==alreadyChecked[i]) return;
		}
		alreadyChecked.push_back(other);
		other->alreadyChecked.push_back(this);
	}
	onAABBintersect(other,time,tsp);
	//TODO empty vector when calculating new aabb and aabb is multichunk
}
#include <iostream>
void Entity::onAABBintersect(Entity* other, float time,TickServiceProvider* tsp)
{
	std::cout<<"warning: onAABBintersect not overridden"<<std::endl;
}

Entity::~Entity()
{
}

Pushable* Entity::toPushable()
{
	return 0;
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
