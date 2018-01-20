/*
 * Compatibility.cpp
 *
 *  Created on:	12.12.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "Compatibility.h"


Compatibility::Compatibility(int type, int size):
ID((size<<24)+(type&16777215))
{}

Compatibility::Compatibility()
{}

Compatibility::~Compatibility()
{}

bool Compatibility::fitsInsideMe(Compatibility other)
{
	int myType=ID<<8;
	int otherType=other.ID<<8;
	if(myType!=otherType) return false;
	int mySize=ID>>24;
	int otherSize=other.ID>>24;
	return otherSize<=mySize;
}

Compatibility::Compatibility(unsigned int typeSizeCombined):
ID(typeSizeCombined)
{}

bool Compatibility::doIFitInside(Compatibility other)
{
	return other.fitsInsideMe(*this);
}
