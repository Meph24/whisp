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

/*
vec3 Entity::getRelPosOf(Entity* other)
{
	return other->pos-pos;//can change if pos gets relative to current chunk
}
*/

Entity::~Entity()
{}
/*
void Entity::onAABBintersect(Entity* other)
{}

bool Entity::aabbIntersects(Entity* other)
{
	vec3 myAbsLow=pos+aabbOlow;
	vec3 myAbsHigh=pos+aabbOhigh;
	vec3 otherAbsLow=other->pos+other->aabbOlow;
	vec3 otherAbsHigh=other->pos+other->aabbOhigh;
	unsigned int otherIsLower=myAbsLow>otherAbsHigh;
	unsigned int otherIsHigher=myAbsHigh<otherAbsLow;
	return !(otherIsLower||otherIsHigher);
}*/
