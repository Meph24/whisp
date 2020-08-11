/*
 * BenchScenarioParam.cpp
 *
 *  Created on:	13.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchScenarioParam.h"

float BenchScenarioParam::getCurValue()
{
	return values[indx];
}

bool BenchScenarioParam::next(float seconds)
{
	bool skip=true;
	if(!skipWhenTooMuchTime) skip=false;//no limit set
	if(seconds<=maxTime) skip=false;//limit obeyed
	indx++;
	if(indx>=values.size() || skip)
	{
		indx=0;
		return true;
	}
	return false;
}

void BenchScenarioParam::addValue(float value)
{
	values.push_back(value);
}

BenchScenarioParam::BenchScenarioParam(std::string Name,float MaxTime):
skipWhenTooMuchTime(MaxTime>=0),maxTime(MaxTime),name(Name)
{}

BenchScenarioParam::~BenchScenarioParam()
{
}

BenchScenarioParam::operator float()
{
	return getCurValue();
}
