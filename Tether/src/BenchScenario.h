/*
 * BenchScenario.h
 *
 *  Created on:	12.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHSCENARIO_H_
#define SRC_BENCHSCENARIO_H_

#include "BenchScenarioParam.h"

#include <string>

class IWorld;

class BenchScenario
{
protected:
	float randomFloat();
	BenchScenarioParam absoluteSize;//average size of object in largest dimension
	BenchScenarioParam objectCount;
public:
	std::string name="unnamed";
	std::vector<BenchScenarioParam *> params;

	BenchScenario();
	virtual ~BenchScenario();

	virtual bool prepareNextParameters(float seconds);//true means finished
	virtual void spawnEntities(IWorld * w)=0;
	virtual bool isSym()=0;

	int getSeed();


	friend std::ostream& operator<<(std::ostream &out,const BenchScenario& s);
};

std::ostream& operator<<(std::ostream& os, const BenchScenario& s);

#endif /* SRC_BENCHSCENARIO_H_ */
