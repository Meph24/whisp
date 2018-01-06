/*
 * RecoilReturning.cpp
 *
 *  Created on:	05.01.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "RecoilReturning.h"

vec3 RecoilReturning::getRecoil(float time)
{
	float elapsed=time/totalLength;
	float firstTimer=0.05f;
	if(elapsed<firstTimer) return (elapsed/firstTimer)*vec3(1,1,1);
	if(elapsed>=1) return vec3(1,1,1)-retP;
	elapsed-=firstTimer;
	elapsed/=(1-firstTimer);
	return vec3(1,1,1)-retP*elapsed;
}

RecoilReturning::RecoilReturning(vec3 returnPart, float fullyReturnTime,float fastestShotTime):
RecoilManager((int)(fullyReturnTime/fastestShotTime)+1,fullyReturnTime),retP(returnPart)
{
}

RecoilReturning::~RecoilReturning()
{
}

