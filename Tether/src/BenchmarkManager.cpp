/*
 * BenchmarkManager.cpp
 *
 *  Created on:	04.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchmarkManager.h"
#include "IWorld.h"

BenchmarkManager::BenchmarkManager(IWorld* World):
world(World)
{
}

void BenchmarkManager::tick(Timestamp t, TickServiceProvider* tsp)
{
}

BenchmarkManager::~BenchmarkManager()
{
	// TODO Auto-generated destructor stub
}

