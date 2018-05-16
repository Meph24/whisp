/*
 * AdaptiveQuality.cpp
 *
 *  Created on:	04.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "AdaptiveQuality.h"
#include <math.h>

AdaptiveQuality::AdaptiveQuality(float minQuality,float maxQuality,float targetTime):
target(targetTime),max(maxQuality),min(minQuality),lastQuality(maxQuality)
{}

AdaptiveQuality::~AdaptiveQuality()
{}

//#include <iostream>
#define LIFT_RESTRICTION_VALUE 2.0f

//TODO make it work with externally limited framerate
float AdaptiveQuality::getQuality(float curTime)
{
	float correctionDir=target/curTime;
	float newQ=lastQuality;

	if(correctionDir<1)
	{
		newQ=lastQuality*correctionDir*correctionDir;
	}
	else if(correctionDir>LIFT_RESTRICTION_VALUE)
	{
		newQ=lastQuality*sqrt(sqrt(sqrt(correctionDir/LIFT_RESTRICTION_VALUE)));
	}
	if(newQ>max) newQ=max;
	else if(newQ<min) newQ=min;
	lastQuality=newQ;
//	std::cout<<"renderTime="<<curTime<<std::endl;
//	std::cout<<"newQ="<<newQ<<std::endl;
	return newQ;
}
