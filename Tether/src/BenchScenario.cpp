/*
 * BenchScenario.cpp
 *
 *  Created on:	12.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchScenario.h"

#include <cstdlib>

BenchScenario::BenchScenario():
runNumber("runNumber"),absoluteSize("absoluteSize"),objectCount("objectCount",0.25f)
{
	for(int i=0;i<5;i++)
	{
		runNumber.addValue(i);
	}
	params.push_back(&runNumber);

	objectCount.addValue(128);
	objectCount.addValue(256);
	objectCount.addValue(512);
	objectCount.addValue(1024);
	objectCount.addValue(2048);
	objectCount.addValue(4096);
	objectCount.addValue(8192);
	objectCount.addValue(16384);
	objectCount.addValue(32768);
	params.push_back(&objectCount);

	absoluteSize.addValue(10);
	absoluteSize.addValue(2);
	absoluteSize.addValue(1);
	absoluteSize.addValue(0.5);
	absoluteSize.addValue(0.1);
	params.push_back(&absoluteSize);
}

BenchScenario::~BenchScenario()
{
	for(auto ptr: params) delete ptr;
}

float BenchScenario::randomFloat()
{
	float lsBits=rand()/(float)RAND_MAX;
	return (rand()+lsBits)/((float)RAND_MAX+1);
}

#include "myAssert.h"

bool BenchScenario::prepareNextParameters(float seconds)
{
	for(auto p: params)
	{
		assert(p);
		if(!p->next(seconds)) return false;
	}
	return true;
}
#include <iostream>

int BenchScenario::getSeed()
{
	int nextMult=1;
	int ret=0;
	for(BenchScenarioParam * param: params)
	{
		ret+=nextMult*param->getSeed();
		nextMult*=param->getSeedMax();
	}
	ret+=nextMult*name.at(0);
	return ret;
}

std::ostream& operator <<(std::ostream& out, const BenchScenario& s)
{
	out<<"scenarioName = '"<<s.name<<"', scenarioParameters = { ";
	bool needComma=false;
	for(BenchScenarioParam * p: s.params)
	{

		if(needComma) out<<", ";
		out<<"'"<<p->name<<"' : "<<p->getCurValue();

		if(!needComma) needComma=true;
	}
	out<<" }";
	return out;
}
