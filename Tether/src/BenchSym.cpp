/*
 * BenchSym.cpp
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchSym.h"

#include "TickServiceProvider.h"

BenchSym::BenchSym()
{
}

void BenchSym::interact(Entity* self, DualPointer<BenchSym> other, float time,TickServiceProvider& tsp)
{
	tsp.interactionCounter++;
	tsp.arbitraryNumber+=(unsigned long long) self;
	tsp.arbitraryNumber+=(unsigned long long) other.e;
	//by doing all this the algorithm cannot be optimized away by the compiler
}

BenchSym::~BenchSym()
{
}

