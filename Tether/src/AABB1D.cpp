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
	bool otherIsLower=low>bb.high.*dim;
	bool otherIsHigher=high<bb.low.*dim;
	return !(otherIsLower|otherIsHigher);
}

bool AABB1D::doesIntersect(spacelen spacevec::*dim, AABB* bb)
{
	return doesIntersect(dim,*bb);
}

void AABB1D::extend(spacelen spacevec::*dim, AABB bb)
{
	spacelen otherLow=bb.low.*dim;
	spacelen otherHigh=bb.high.*dim;
	auto lower=otherLow<low;
	auto higher=otherHigh>high;
	low.intpart=otherLow.intpart*(lower)+low.intpart*(!lower);
	low.floatpart=otherLow.floatpart*(lower)+low.floatpart*(!lower);
	high.intpart=otherHigh.intpart*(higher)+high.intpart*(!higher);
	high.floatpart=otherHigh.floatpart*(higher)+high.floatpart*(!higher);
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

