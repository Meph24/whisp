/*
 * AnimationCycle.cpp
 *
 *  Created on:	Apr 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "AnimationCycle.h"

void AnimationCycle::updateInternals(float timePassed)
{
	progress+=timePassed*cycleLenInv;
	progress=cap(progress);
}

float AnimationCycle::cap(float x)
{
	int rounds=(int)x;
	if(remaining<0)//no limit
	{
		x-=rounds;
		if(x<0) x+=1;
		return x;
	}
	else if(remaining>rounds)//inside limit
	{
		remaining-=rounds;
		x-=rounds;
		if(x<0) x+=1;
		return x;
	}
	//else: //outside limit
	if(rounds==0)
	{
		if(x<0) x+=1;
		return x;
	}
	else if(rounds>0)
	{
		return 1;
	}
	else return 0;
}

void AnimationCycle::update(float timePassed)
{
	updateInternals(timePassed-hiddenProgress);
	hiddenProgress=0;
}

void AnimationCycle::updateTemp(float timeOffset)
{
	updateInternals(timeOffset-hiddenProgress);
	hiddenProgress=timeOffset;
}

float AnimationCycle::getCurStep(float phaseOffset)
{
	float myProgress=progress+phaseOffset;
	return cap(myProgress);
}
#include "MathStuff.h"
float AnimationCycle::getCurStepTau(float phaseOffset)
{
	return getCurStep(phaseOffset)*TAU;
}


AnimationCycle::AnimationCycle(float cycleLength, float startPhase,int maxCycles):
		cycleLenInv(1.0f/cycleLength),progress(startPhase),hiddenProgress(0),remaining(maxCycles)
{
	if(remaining>0) remaining--;
}

AnimationCycle::~AnimationCycle()
{}

