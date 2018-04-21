/*
 * Frustum.cpp
 *
 *  Created on:	20.04.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "Frustum.h"

Frustum::Frustum()
{}

bool Frustum::inside(spacelen* bb)
{
	for(int i=0;i<FRUSTUM_PLANE_COUNT;i++)
	{
		if(!planes[i].inside(bb,observerPos)) return false;
	}
	return true;
}

bool Frustum::inside(AABB bb)
{
	return inside(&bb.low.x);
}

Frustum::~Frustum()
{}

