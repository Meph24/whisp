/*
 * AABB.cpp
 *
 *  Created on:	Mar 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "AABB.h"
#include "Frustum.h"
#include "ChunkManager.h"
#include "ICamera3D.h"
#include "DrawServiceProvider.h"
#include <GL/glew.h>
#include "glmutils.hpp"

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
	if(sizeFromMid.x.intpart>=100||sizeFromMid.z.intpart>=100)//TODO find the bug that causes this and remove afterwards
	{
		std::cout<<"REPORT THIS BUG, IT WAS BELIEVED TO BE FIXED AND APPARENTLY IS NOT:"<<std::endl;
		std::cout<<sizeFromMid<<std::endl;
		std::cout<<pos<<std::endl;
		assert(sizeFromMid.x.intpart<100);
		assert(sizeFromMid.z.intpart<100);
	}
	assert(!(low>high));
}

AABB::AABB(spacevec posT1, spacevec sizeFromMid, spacevec movement):
low(posT1-sizeFromMid),high(posT1+sizeFromMid)
{
	if(sizeFromMid.x.intpart>=100||sizeFromMid.z.intpart>=100||movement.z.intpart>=100||movement.x.intpart>=100)//TODO find the bug that causes this and remove afterwards
	{
		std::cout<<"REPORT THIS BUG, IT WAS BELIEVED TO BE FIXED AND APPARENTLY IS NOT:"<<std::endl;
		std::cout<< "pos_t1:\n" << posT1<<std::endl;
		std::cout<< "fromMid:\n" << sizeFromMid<<std::endl;
		std::cout<< "movement:\n" << movement<<std::endl;

		assert(sizeFromMid.x.intpart<100);
		assert(sizeFromMid.z.intpart<100);
		assert(movement.z.intpart<100);//Assertion failed! Expression: movement.z.intpart<10
		assert(movement.x.intpart<100);
	}
	spacevec zero;
	zero.set0();
	low-=movement.selectWhere(movement>zero);//if moved in + direction, posT0 was in - direction before
	high-=movement.selectWhere(movement<zero);//if moved in - direction, posT0 was in + direction before
	if(low>high)
	{
		std::cout<<posT1<<","<<sizeFromMid<<","<<movement<<","<<low<<","<<high<<std::endl;
	}
	assert(!(low>high));
}

bool AABB::isMultichunk()
{
	return (low.x.intpart!=high.x.intpart)||(low.y.intpart!=high.y.intpart)||(low.z.intpart!=high.z.intpart);
}

void AABB::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum, IWorld& iw,DrawServiceProvider* dsp)
{
	if(!viewFrustum->inside(*(this),iw)) return;
	float widthOnScreen=1.0f/1024;//apparent size on the screen
	float maxWidthPortion=0.4f;//max opaque portion (0-1), for distant objects

	spacevec sizeHalf=(high-low)/2.0f;
	spacevec mid=low+sizeHalf;

	spacevec interPos=mid-viewFrustum->observerPos;
	vec3 interPosMeters=iw.toMeters(interPos);

	float dist=glm::length(interPosMeters-dsp->getCamPos());

	float width=widthOnScreen*dist;
	vec3 distMax=iw.toMeters(sizeHalf);
	float maxAbsoluteSize=distMax.x;
	maxAbsoluteSize=distMax.y<maxAbsoluteSize?distMax.y:maxAbsoluteSize;
	maxAbsoluteSize=distMax.z<maxAbsoluteSize?distMax.z:maxAbsoluteSize;
	maxAbsoluteSize*=maxWidthPortion;

	width=width>maxAbsoluteSize?maxAbsoluteSize:width;

	vec3 distMin=distMax-vec3(width,width,width);

	glPushMatrix();
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);

//	glColor3f(1,1,1);
//	std::cout<<std::endl;
	for(int i=0;i<3;i++)
	{
		int firCoo=1<<i;
		int secCoo=(firCoo<<1)&7;
		secCoo=secCoo==0?1:secCoo;
		int terCoo=(secCoo<<1)&7;
		terCoo=terCoo==0?1:terCoo;

		for(int j=0;j<4;j++)
		{
			int firCooMult=(j&1)*2-1;
			int secTerCooMult=(j&2)-1;
			vec3 fir=convert(firCoo)*(float)firCooMult;
			vec3 sec=convert(secCoo)*(float)secTerCooMult;
			vec3 ter=convert(terCoo)*(float)secTerCooMult;
//			std::cout<<"AABB draw: "<<fir<<" "<<sec<<" "<<ter<<std::endl;

			vec3 baseOut=(fir+sec+ter)*distMax;
			vec3 baseIn=(fir*distMax+(sec+ter)*distMin);

			vec3 arm1Out=(fir-sec+ter)*distMax;
			vec3 arm1In=(fir*distMax+(ter-sec)*distMin);

			vec3 arm2Out=(fir+sec-ter)*distMax;
			vec3 arm2In=(fir*distMax+(sec-ter)*distMin);

			glBegin(GL_TRIANGLE_FAN);

			vec3
			curVec=baseOut; glVertex3f(curVec.x,curVec.y,curVec.z);
			curVec=arm1Out; glVertex3f(curVec.x,curVec.y,curVec.z);
			curVec=arm1In; glVertex3f(curVec.x,curVec.y,curVec.z);
			curVec=baseIn; glVertex3f(curVec.x,curVec.y,curVec.z);
			curVec=arm2In; glVertex3f(curVec.x,curVec.y,curVec.z);
			curVec=arm2Out; glVertex3f(curVec.x,curVec.y,curVec.z);

			glEnd();
		}
	}

//	std::cout<<"AABB size: "<<distMax<<" "<<distMin<<" "<<width<<" "<<dist<<std::endl;
	glPopMatrix();
}

vec3 AABB::convert(int bitvec)
{
	float x=bitvec&1;
	float y=(bitvec&2)/2;
	float z=(bitvec&4)/4;
	return vec3(x,y,z);
}

void AABB::extend(AABB other)
{
	spacevec otherLow=other.low;
	spacevec otherHigh=other.high;
	auto lower=otherLow<low;
	auto higher=otherHigh>high;
	low=otherLow.selectWhere(lower)+low.selectWhere(~lower);
	high=otherHigh.selectWhere(higher)+high.selectWhere(~higher);
}

void AABB::extend(AABB* other)
{
	extend(*other);
}

AABB::~AABB()
{}

std::ostream& operator<< (std::ostream& os, const AABB& bb)
{
	os << bb.low << "|" << bb.high;
	return os;
}

bool AABB::contains(spacevec s)
{
	return (!(low>s))&&(!(high<s));
}

AABB::AABB(spacevec posT0, spacevec posT1, spacevec lowBorderOffset,spacevec highBorderOffset)
{
	assert(highBorderOffset.x>=lowBorderOffset.x);
	assert(highBorderOffset.y>=lowBorderOffset.y);
	assert(highBorderOffset.z>=lowBorderOffset.z);
	low=posT0.selectWhere(posT0<posT1)+posT1.selectWhere(posT1<=posT0);
	high=posT0.selectWhere(posT0>posT1)+posT1.selectWhere(posT1>=posT0);
	low+=lowBorderOffset;
	high+=highBorderOffset;
	assert(!(low>high));
}
