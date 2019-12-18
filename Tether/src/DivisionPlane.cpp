/*
 * DivisionPlane.cpp
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "DivisionPlane.h"
#include "IWorld.h"

DivisionPlane::DivisionPlane(vec3 Normal,float Dist):
normal(Normal),distanceInMeters(Dist)
{
	if(normal.x<0)
	{
		xPtr=&AABB::high;
	}
	else
	{
		xPtr=&AABB::low;
	}
	if(normal.y<0)
	{
		yPtr=&AABB::high;
	}
	else
	{
		yPtr=&AABB::low;
	}
	if(normal.z<0)
	{
		zPtr=&AABB::high;
	}
	else
	{
		zPtr=&AABB::low;
	}
}

DivisionPlane::~DivisionPlane()
{
}

bool DivisionPlane::inside(AABB bb,spacevec observerPos,IWorld& w)
{
	spacevec relevantPoint;
	relevantPoint.x=(bb.*xPtr).x-observerPos.x;
	relevantPoint.y=(bb.*yPtr).y-observerPos.y;
	relevantPoint.z=(bb.*zPtr).z-observerPos.z;
	float d=glm::dot(w.toMeters(relevantPoint),normal);
	return d<distanceInMeters;
}


DivisionPlane::DivisionPlane()
{
}


std::ostream& operator<< (std::ostream& os, const DivisionPlane& dp)
{
	os << "DivPlane : Normal[" << dp.normal << "], distanceInMeters[" << dp.distanceInMeters << "]";
	return os;
}
