/*
 * TimestampManager.cpp
 *
 *  Created on:	28.04.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "TimestampManager.h"

TimestampManager::TimestampManager(float TargetRate,float MaxMasterWallTime,float MaxMasterGameTime):
maxMasterWallTime(MaxMasterWallTime*1000),maxMasterGameTime(MaxMasterGameTime*1000),targetRate(TargetRate)
{
	lastMasterTime=getCurrentTime();
	lastMasterTimestamp=Timestamp(0);
}

i64 TimestampManager::getCurrentTime()
{
	return clock.getElapsedTime().asMicroseconds();
}
#include "WarnErrReporter.h"
Timestamp TimestampManager::masterUpdate()
{
	i64 curTime=getCurrentTime();
	i64 elapsedReal=curTime-lastMasterTime;
	if(elapsedReal<0)
	{
		WarnErrReporter::timeBackwardErr("found a time elapsed <0");
		elapsedReal=0;
	}
	float elapsed=elapsedReal>maxMasterWallTime?maxMasterWallTime:elapsedReal;
	elapsed*=targetRate;
	elapsed=elapsed>maxMasterGameTime?maxMasterGameTime:elapsed;
	lastMasterRate=elapsed/elapsedReal;
	lastMasterTimestamp.time+=(int)elapsed;
	lastMasterTime=curTime;
	return lastMasterTimestamp;
}
//#include <iostream>
Timestamp TimestampManager::getSlaveTimestamp()
{
	i64 curTime=getCurrentTime();
	i64 elapsedReal=curTime-lastMasterTime;
	if(elapsedReal<0)
	{
		WarnErrReporter::timeBackwardErr("found a time elapsed <0");
		elapsedReal=0;
	}
	float elapsed=elapsedReal;
	elapsed*=lastMasterRate;
//	std::cout<<lastMasterRate<<std::endl;
	Timestamp ret=lastMasterTimestamp;
	ret.time+=(int)elapsed;
	return ret;
}

TimestampManager::~TimestampManager()
{
}

