/*
 * BenchScenarioUniformAsym.cpp
 *
 *  Created on:	30.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchScenarioUniformAsym.h"

#include "BenchEntityMaster.h"
#include "BenchEntitySlave.h"

BenchScenarioUniformAsym::BenchScenarioUniformAsym():
fraction("MasterIF fraction")
{
	fraction.addValue(0.9);
	fraction.addValue(0.75);
	fraction.addValue(0.5);
	fraction.addValue(0.25);
	fraction.addValue(0.1);
	params.push_back(&fraction);
}

BenchScenarioUniformAsym::~BenchScenarioUniformAsym()
{}

void BenchScenarioUniformAsym::spawnEntities(IWorld* w)
{
	spawnUniform<BenchEntityMaster>((unsigned int) (objectCount*fraction),w);
	spawnUniform<BenchEntitySlave>((unsigned int) (objectCount*(1-fraction)),w);
}

bool BenchScenarioUniformAsym::isSym()
{
	return false;
}
