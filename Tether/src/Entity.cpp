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
{}


