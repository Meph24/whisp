/*
 * IWorld.cpp
 *
 *  Created on:	09.09.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "IWorld.h"

IWorld::IWorld()
{
	// TODO Auto-generated constructor stub
}

IWorld::~IWorld()
{
	// TODO Auto-generated destructor stub
}

float IWorld::toMeters(spacelen l)
{
	float ret=l.floatpart;
	ret+=l.intpart;
	ret*=gridSize;
	return ret;
}
double IWorld::toMetersD(spacelen l)
{
	double ret=l.floatpart;
	ret+=l.intpart;
	ret*=gridSize;
	return ret;
}

vec3 IWorld::toMeters(spacevec v)
{
	vec3 ret=vec3(v.x.floatpart,v.y.floatpart,v.z.floatpart);
	ret+=vec3(v.x.intpart,v.y.intpart,v.z.intpart);
	ret=ret*gridSize;
	return ret;
}

spacelen IWorld::fromMeters(float l) const
{
	l/=gridSize;
	spacelen ret;
	ret.floatpart=l;
	ret.intpart=0;
	ret.correct();
	return ret;
}

spacevec IWorld::fromMeters(const glm::vec3& v) const
{
	spacevec ret;
	ret.x=fromMeters(v.x);
	ret.y=fromMeters(v.y);
	ret.z=fromMeters(v.z);
	return ret;
}
