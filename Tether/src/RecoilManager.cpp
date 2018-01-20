/*
 * RecoilManager.cpp
 *
 *  Created on:	05.01.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "RecoilManager.h"

RecoilManager::RecoilManager(int arraySize,float recoilLength):
size(arraySize),start(0),curLen(0),lastRecoil({0,0,0}),totalLength(recoilLength)
{
	timer=new float[size];
	recoilComponents=new vec3[size*3];
}

RecoilManager::~RecoilManager()
{
	delete timer;
	delete recoilComponents;
}

vec3 RecoilManager::getCurrentRecoil(float timePassed)
{
	int i=start;
	if(curLen) if(timer[i]>=totalLength)
	{
		vec3 rec;
		vec3 thisRecoil=getRecoil(totalLength);
		vec3 * myRecoilPart=recoilComponents+i*3;
		rec=thisRecoil.x*myRecoilPart[0]+thisRecoil.y*myRecoilPart[1]+thisRecoil.z*myRecoilPart[2];
		lastRecoil-=rec;
		i=start=incr(start);
		curLen--;
	}
	vec3 ret={0,0,0};
	for(int j=0;j<curLen;j++)
	{
		float newTimer=timer[i]+timePassed;
		vec3 thisRecoil=getRecoil(newTimer);
		vec3 * myRecoilPart=recoilComponents+i*3;
		ret+=thisRecoil.x*myRecoilPart[0]+thisRecoil.y*myRecoilPart[1]+thisRecoil.z*myRecoilPart[2];
		timer[i]=newTimer;
		i=incr(i);
	}
	return ret;
}

vec3 RecoilManager::getCurrentRecoil()
{
	return getCurrentRecoil(0);
}

int RecoilManager::incr(int pos)
{
	return (pos+1)%size;
}

int RecoilManager::incr(int pos, int by)
{
	return (pos+by)%size;
}

vec3 RecoilManager::getRecoilDiff(float timePassed)
{
	vec3 newRecoil=getCurrentRecoil(timePassed);
	vec3 ret=newRecoil-lastRecoil;
	lastRecoil=newRecoil;
	return ret;
}

#include "myAssert.h"
void RecoilManager::registerRecoil(vec3 component1,vec3 component2,vec3 component3)
{
	assert(curLen<size);
	int myInd=incr(start,curLen);
	timer[myInd]=0;
	vec3 * myRecoilPart=recoilComponents+myInd*3;
	myRecoilPart[0]=component1;
	myRecoilPart[1]=component2;
	myRecoilPart[2]=component3;
	curLen++;
}
