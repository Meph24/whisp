/*
 * AnimationCycle.cpp
 *
 *  Created on:	Apr 12, 2018
 *      Author:	HL65536
 *     Version:	2.1
 */

#include "AnimationCycle.h"

float AnimationCycle::modFloat(float x, float by)
{
	int a=(int)(x/by);
	if(x<0) a--;
	return x-a*by;
}

float AnimationCycle::cap01(float x)
{
	if(maxCycles==0)//no limit
	{
		return modFloat(x,1);
	}
	else if(maxCycles>x && x>=0)//inside limit
	{
		return modFloat(x,1);
	}
	//else: //outside limit
	if(x>=maxCycles) return 1;
	return 0;//x<0
}

float AnimationCycle::cap0max(float x)
{
	if(maxCycles==0)//no limit
	{
		return modFloat(x,1);
	}
	else if(maxCycles>x && x>=0)//inside limit
	{
		return x;
	}
	//else: //outside limit
	if(x>=maxCycles) return maxCycles;
	return 0;//x<0
}

void AnimationCycle::update(float timePassed)
{
	confirmedProgress+=timePassed*cycleLenInv;
	confirmedProgress=cap0max(confirmedProgress);
}

void AnimationCycle::updateTemp(float timeOffset)
{
	tempProgress=timeOffset*cycleLenInv;
}

float AnimationCycle::getCurStep(float phaseOffset,bool withTempProgress)
{
	float myProgress=confirmedProgress+phaseOffset;
	if(withTempProgress) myProgress+=tempProgress;
	return cap01(myProgress);
}
#include "MathStuff.h"
float AnimationCycle::getCurStepTau(float phaseOffset,bool withTempProgress)
{
	return getCurStep(phaseOffset,withTempProgress)*TAU;
}


AnimationCycle::AnimationCycle(float cycleLength, float startPhase,u8 maxCyc)
: tempProgress(0)
, maxCycles(maxCyc)
, cycleLenInv(1.0f/cycleLength)
, confirmedProgress(startPhase)
{
}

AnimationCycle::~AnimationCycle()
{
}

void AnimationCycle::serialize(sf::Packet& p, bool complete)
{
	p<<confirmedProgress;
	if(complete)
	{
		p<<maxCycles;
		p<<cycleLenInv;
	}
}

void AnimationCycle::deserialize(sf::Packet& p, SyncableManager& sm)
{
	p>>confirmedProgress;
}

AnimationCycle::AnimationCycle(sf::Packet p, SyncableManager& sm)
{
	deserialize(p,sm);
	p>>maxCycles;
	p>>cycleLenInv;
}

