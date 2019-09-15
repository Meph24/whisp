/*
 * DivisionPlane.cpp
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "DivisionPlane.h"

DivisionPlane::DivisionPlane(vec3 Normal,flt Dist):
normal(Normal),distanceInChunks(Dist)
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

bool DivisionPlane::inside(spacelen * bb,spacevec observerPos)
{
	spacevec relevantPoint;
	relevantPoint.x=*(bb+xPtr)-observerPos.x;
	relevantPoint.y=*(bb+yPtr)-observerPos.y;
	relevantPoint.z=*(bb+zPtr)-observerPos.z;
	flt dot=relevantPoint.dot(normal);
	return dot<distanceInChunks;
}

bool DivisionPlane::inside(AABB bb,spacevec observerPos)
{
	return inside(&bb.low.x,observerPos);
}

DivisionPlane::DivisionPlane()
{
}


std::ostream& operator<< (std::ostream& os, const DivisionPlane& dp)
{
	os << "DivPlane : Normal[" << dp.normal << "], distanceInChunks[" << dp.distanceInChunks << "]";
	return os;
}
