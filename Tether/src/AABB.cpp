/*
 * AABB.cpp
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "AABB.h"

AABB::AABB()
{
	// TODO Auto-generated constructor stub

}

void AABB::addSpeed(vec3 displacement)
{

}

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

void AABB::operator +=(AABB other)
{
	(*this)+=other.high;
	(*this)+=other.low;
}

void AABB::operator +=(AABB* other)
{
	(*this)+=other[0].high;
	(*this)+=other[0].low;
}

void AABB::operator +=(vec3 point)
{
	//TODO if point.x
}

void AABB::operator +=(vec3* point)
{
	(*this)+=point[0];
}

AABB::~AABB()
{
	// TODO Auto-generated destructor stub
}

