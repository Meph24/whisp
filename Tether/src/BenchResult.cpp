/*
 * BenchResult.cpp
 *
 *  Created on:	12.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchResult.h"

#include <algorithm>

BenchResult::BenchResult(std::vector<float>& rawData)
{
	sort(rawData.begin(),rawData.end());
	auto size=rawData.size();
	min=rawData[0];
	max=rawData[size-1];
	median=rawData[size/2];
	avg=0;
	for(float t: rawData) avg+=t;
	avg/=size;
	rawData.clear();
}

BenchResult::~BenchResult()
{
}

