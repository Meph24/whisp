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
//	spacelen unitLength;
//	unitLength.floatpart=0;
//	unitLength.intpart=1;
//
//	float averageObjectSizeMeters=w->toMeters(unitLength)*absoluteSize;
//	float averageDistanceMeters=averageObjectSizeMeters/density;
//	float spawnAreaSize=averageDistanceMeters*cbrt(objectCount);
//
//	for(int i=0;i<objectCount;i++)
//	{
//		vec3 positionVec(randomFloat(),randomFloat(),randomFloat());
//		positionVec*=spawnAreaSize;
//
//		vec3 sizeVec(randomFloat(),randomFloat(),randomFloat());
//		float largest=sizeVec.x;
//		if(sizeVec.y>largest) largest=sizeVec.y;
//		if(sizeVec.z>largest) largest=sizeVec.z;
//		sizeVec/=largest;//largest extent is now always 1
//
//		float sizeMeters=randomFloat()*2*averageObjectSizeMeters;
//		sizeVec*=sizeMeters;
//
//		w->requestEntitySpawn(new BenchEntityS(w->fromMeters(sizeVec),w->fromMeters(positionVec)));
//	}
}

bool BenchScenarioUniform::isSym()
{
	return true;
}
