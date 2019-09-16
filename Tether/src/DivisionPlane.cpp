/*
 * DivisionPlane.cpp
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "DivisionPlane.h"
#include "IWorld.h"

DivisionPlane::DivisionPlane(vec3 Normal,flt Dist):
normal(Normal),distanceInMeters(Dist)
{
	AABB test;
	spacelen * ref=&(test.low.x);
	if(normal.x<0)
	{
		xPtr=&(test.high.x)-ref;
	}
	else
	{
		xPtr=&(test.low.x)-ref;
	}
	if(normal.y<0)
	{
		yPtr=&(test.high.y)-ref;
	}
	else
	{
		yPtr=&(test.low.y)-ref;
	}
	if(normal.z<0)
	{
		zPtr=&(test.high.z)-ref;
	}
	else
	{
		zPtr=&(test.low.z)-ref;
	}
}

DivisionPlane::~DivisionPlane()
{
}

bool DivisionPlane::inside(spacelen * bb,spacevec observerPos,IWorld * w)
{
	spacevec relevantPoint;
	relevantPoint.x=*(bb+xPtr)-observerPos.x;
	relevantPoint.y=*(bb+yPtr)-observerPos.y;
	relevantPoint.z=*(bb+zPtr)-observerPos.z;
	flt dot=dotProduct(w->toMeters(relevantPoint),normal);
	return dot<distanceInMeters;
}

bool DivisionPlane::inside(AABB bb,spacevec observerPos,IWorld * w)
{
	return inside(&bb.low.x,observerPos,w);
}

DivisionPlane::DivisionPlane()
{
}
