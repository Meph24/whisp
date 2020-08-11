/*
 * BenchScenarioShotgun.h
 *
 *  Created on:	14.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHSCENARIOSHOTGUN_H_
#define SRC_BENCHSCENARIOSHOTGUN_H_

#include "BenchScenario.h"

class BenchScenarioShotgun: public BenchScenario
{
	BenchScenarioParam projectileCountFraction;//fraction of objects being projectiles
	BenchScenarioParam patternVictimNumber;//expected number of victim objects in spawn area of projectiles

public:
	BenchScenarioShotgun();
	virtual ~BenchScenarioShotgun();
	virtual void spawnEntities(IWorld * w);
	virtual bool isSym();
};

#endif /* SRC_BENCHSCENARIOSHOTGUN_H_ */
