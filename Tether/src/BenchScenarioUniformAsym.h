/*
 * BenchScenarioUniformAsym.h
 *
 *  Created on:	30.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHSCENARIOUNIFORMASYM_H_
#define SRC_BENCHSCENARIOUNIFORMASYM_H_

#include "BenchScenarioUniform.h"

class BenchScenarioUniformAsym: public BenchScenarioUniform
{
	BenchScenarioParam fraction;//fraction of objects being of MasterIF type

public:
	BenchScenarioUniformAsym();
	virtual ~BenchScenarioUniformAsym();
	void spawnEntities(IWorld * w);
	bool isSym();
};

#endif /* SRC_BENCHSCENARIOUNIFORMASYM_H_ */
