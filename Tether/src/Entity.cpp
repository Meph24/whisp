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


Entity::~Entity()
{}

void Entity::onAABBintersect(Entity* other)
{
}

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
