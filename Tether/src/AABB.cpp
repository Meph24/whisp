/*
 * AABB.cpp
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "AABB.h"

u64 AABB::intersectionCounter=0;
u64 AABB::checkCounter=0;

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

AABB::AABB()
{}

AABB::AABB(spacevec pos):
low(pos),high(pos)
{}

#include "myAssert.h"
AABB::AABB(spacevec pos, spacevec sizeFromMid):
low(pos-sizeFromMid),high(pos+sizeFromMid)
{
	if(sizeFromMid.x.intpart>=10||sizeFromMid.z.intpart>=10)//TODO find the bug that causes this and remove afterwards
	{
		std::cout<<"REPORT THIS BUG, IT IS RARE AND NEEDS TO BE FIXED:"<<std::endl;
		std::cout<<sizeFromMid<<std::endl;
		std::cout<<pos<<std::endl;
		assert(sizeFromMid.x.intpart<10);
		assert(sizeFromMid.z.intpart<10);
	}
}

AABB::AABB(spacevec pos, spacevec sizeFromMid, spacevec movement):
low(pos-sizeFromMid),high(pos+sizeFromMid)
{
	if(sizeFromMid.x.intpart>=10||sizeFromMid.z.intpart>=10||movement.z.intpart>=10||movement.x.intpart>=10)//TODO find the bug that causes this and remove afterwards
	{
		std::cout<<"REPORT THIS BUG, IT IS RARE AND NEEDS TO BE FIXED:"<<std::endl;
		std::cout<<sizeFromMid<<std::endl;
		std::cout<<pos<<std::endl;
		std::cout<<movement<<std::endl;

		assert(sizeFromMid.x.intpart<10);
		assert(sizeFromMid.z.intpart<10);
		assert(movement.z.intpart<10);//Assertion failed! Expression: movement.z.intpart<10
		assert(movement.x.intpart<10);
	}
	spacelen zero;
	zero.floatpart=0;
	zero.intpart=0;
	bool xbig=movement.x>zero;
	bool ybig=movement.y>zero;
	bool zbig=movement.z>zero;
	if(xbig) high.x+=movement.x;
	else low.x+=movement.x;
	if(ybig) high.y+=movement.y;
	else low.y+=movement.y;
	if(zbig) high.z+=movement.z;
	else low.z+=movement.z;
}

bool AABB::isMultichunk()
{
	return (low.x.intpart!=high.x.intpart)||(low.z.intpart!=high.z.intpart);
}

//void AABB::draw(Timestamp t, Frustum* viewFrustum, ChunkManager* cm,DrawServiceProvider* dsp)
//{
//	if(!viewFrustum->inside(*this)) return;
//
//
//}

AABB::~AABB()
{}

