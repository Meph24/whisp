/*
 * AABB1D.cpp
 *
 *  Created on:	04.04.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "AABB1D.h"


bool AABB1D::doesIntersect(spacelen spacevec::*dim, AABB bb)
{
	unsigned int otherIsLower=low>bb.high.*dim;
	unsigned int otherIsHigher=high<bb.low.*dim;
	return !(otherIsLower||otherIsHigher);
}

bool AABB1D::doesIntersect(spacelen spacevec::*dim, AABB* bb)
{
	return doesIntersect(dim,*bb);
}

void AABB1D::extend(spacelen spacevec::*dim, AABB bb)
{
	spacelen otherLow=bb.low.*dim;
	spacelen otherHigh=bb.high.*dim;
	if(otherLow<low) low=otherLow;
	if(otherHigh>high) high=otherHigh;
}

void AABB1D::extend(spacelen spacevec::*dim, AABB* bb)
{
	extend(dim,*bb);
}

AABB1D::AABB1D(spacelen pt):
low(pt),high(pt)
{}

AABB1D::AABB1D()
{
}

AABB1D::~AABB1D()
{}

