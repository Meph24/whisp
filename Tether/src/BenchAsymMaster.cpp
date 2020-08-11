/*
 * BenchAsymMaster.cpp
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchAsymMaster.h"

#include "TickServiceProvider.h"

BenchAsymMaster::BenchAsymMaster()
{
}

BenchAsymMaster::~BenchAsymMaster()
{
}

void BenchAsymMaster::interact(Entity* self, DualPointer<BenchAsymSlave> other,float time, TickServiceProvider& tsp)
{
	tsp.interactionCounter++;
	tsp.arbitraryNumber+=(unsigned long long) self;
	tsp.arbitraryNumber+=(unsigned long long) other.e;
	//by doing all this the algorithm cannot be optimized away by the compiler
}
