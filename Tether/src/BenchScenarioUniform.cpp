/*
 * BenchScenarioUniform.cpp
 *
 *  Created on:	13.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchScenarioUniform.h"

#include "BenchEntityS.h"

#include <cmath>

BenchScenarioUniform::BenchScenarioUniform():
density("density")
{
	name="uniform distribution";
	density.addValue(10);
	density.addValue(2);
	density.addValue(1);
	density.addValue(0.5);
	density.addValue(0.1);
	params.push_back(&density);
}

BenchScenarioUniform::~BenchScenarioUniform()
{}

void BenchScenarioUniform::spawnEntities(IWorld* w)
{
	spawnUniform<BenchEntityS>((unsigned int) objectCount,w);
}

bool BenchScenarioUniform::isSym()
{
	return true;
}
