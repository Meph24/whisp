/*
 * BenchScenarioShotgun.cpp
 *
 *  Created on:	14.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchScenarioShotgun.h"

#include "Spacevec.h"
#include "IWorld.h"
#include "BenchEntityMaster.h"
#include "BenchEntitySlave.h"

#include "BenchmarkTest.h"

#include <cmath>

BenchScenarioShotgun::BenchScenarioShotgun():
projectileCountFraction("projectile count fraction"),patternVictimNumber("pattern victim number")
{
	name="shotgun blast";
	projectileCountFraction.addValue(0.9);
	projectileCountFraction.addValue(0.75);
	projectileCountFraction.addValue(0.5);
	projectileCountFraction.addValue(0.25);
	projectileCountFraction.addValue(0.1);
	params.push_back(&projectileCountFraction);
	patternVictimNumber.addValue(0.125);
	patternVictimNumber.addValue(1);
	patternVictimNumber.addValue(8);
	patternVictimNumber.addValue(64);
	params.push_back(&patternVictimNumber);
}

BenchScenarioShotgun::~BenchScenarioShotgun()
{
}

void BenchScenarioShotgun::spawnEntities(IWorld* w)
{
	float density=1;

	spacelen unitLength;
	unitLength.floatpart=0;
	unitLength.intpart=1;

	float projectileCount=objectCount*projectileCountFraction;//Master
	float victimCount=objectCount-projectileCount;//Slave

	float averageVictimSizeMeters=w->toMeters(unitLength)*absoluteSize;
	float averageDistanceMeters=averageVictimSizeMeters/density;
	float victimSpawnAreaSize=averageDistanceMeters*cbrt(victimCount);

	//BenchmarkTest test;

	for(int i=0;i<victimCount;i++)
	{
		vec3 positionVec(randomFloat(),randomFloat(),randomFloat());
		positionVec*=victimSpawnAreaSize;

		vec3 sizeVec(randomFloat(),randomFloat(),randomFloat());
		float largest=sizeVec.x;
		if(sizeVec.y>largest) largest=sizeVec.y;
		if(sizeVec.z>largest) largest=sizeVec.z;
		sizeVec/=largest;//largest extent is now always 1

		float sizeMeters=randomFloat()*2*averageVictimSizeMeters;
		sizeVec*=sizeMeters;

		BenchEntitySlave * e=new BenchEntitySlave(w->fromMeters(sizeVec),w->fromMeters(positionVec));
		//test.registerEntity(e);
		w->requestEntitySpawn(e);
	}

	//std::cout<<"Slave (victim):\n"<<test;
	//test.reset();

	float projecileSpawnAreaSize=averageDistanceMeters*cbrt(patternVictimNumber);
	vec3 spawnLocVec(randomFloat(),randomFloat(),randomFloat());
	spawnLocVec*=(victimSpawnAreaSize-projecileSpawnAreaSize);

	for(int i=0;i<projectileCount;i++)
	{
		vec3 positionVec1(randomFloat(),randomFloat(),randomFloat());
		vec3 positionVec2(randomFloat(),randomFloat(),randomFloat());

		positionVec1*=projecileSpawnAreaSize;
		positionVec2*=projecileSpawnAreaSize;

		positionVec1+=spawnLocVec;
		positionVec2+=spawnLocVec;

		AABB bb(w->fromMeters(positionVec1));
		bb.extend(w->fromMeters(positionVec2));
		BenchEntityMaster * e = new BenchEntityMaster(bb);
		//test.registerEntity(e);
		w->requestEntitySpawn(e);
	}

	//std::cout<<"Master (projectile):\n"<<test;
}

bool BenchScenarioShotgun::isSym()
{
	return false;
}
