/*
 * MPoint.cpp
 *
 *  Created on:	Aug 14, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "MPoint.h"
MPoint::MPoint()
{
	// TODO Auto-generated constructor stub

}

MPoint::~MPoint()
{
	// TODO Auto-generated destructor stub
}

vec3 MPoint::getRenderPos(float timeSinceUpdate)
{
	return pos+vel*timeSinceUpdate;//TODO add other interpolation functions
}
