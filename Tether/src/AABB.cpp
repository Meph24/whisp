/*
 * AABB.cpp
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "AABB.h"


bool AABB::doesIntersect(AABB other)
{
	unsigned int otherIsLower=low>other.high;
	unsigned int otherIsHigher=high<other.low;
	return !(otherIsLower||otherIsHigher);
}

bool AABB::doesIntersect(AABB * other)
{
	return doesIntersect(other[0]);
}

AABB::AABB(spacevec pos):
low(pos),high(pos)
{
}

AABB::AABB(spacevec pos, spacevec sizeFromMid):
low(pos-sizeFromMid),high(pos+sizeFromMid)
{
}

AABB::AABB(spacevec pos, spacevec sizeFromMid, spacevec movement):
low(pos-sizeFromMid),high(pos+sizeFromMid)
{
	bool xbig=movement.x>spacelen(0,0);
	bool ybig=movement.y>spacelen(0,0);
	bool zbig=movement.z>spacelen(0,0);
	if(xbig) high.x+=movement.x;
	else low.x+=movement.x;
	if(ybig) high.y+=movement.y;
	else low.y+=movement.y;
	if(zbig) high.z+=movement.z;
	else low.z+=movement.z;
}

AABB::~AABB()
{}

