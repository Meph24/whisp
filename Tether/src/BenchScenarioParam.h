/*
 * BenchScenarioParam.h
 *
 *  Created on:	13.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHSCENARIOPARAM_H_
#define SRC_BENCHSCENARIOPARAM_H_

#include <vector>
#include <string>

class BenchScenarioParam
{
	unsigned int indx=0;
	std::vector<float> values;
	bool skipWhenTooMuchTime=false;
	float maxTime;
public:
	std::string name;
	float getCurValue();

	//skips last values if given time>MaxTime
	bool next(float seconds);//returns true if parameter rotation started from beginning again

	void addValue(float value);

	operator float();

	int getSeed();
	int getSeedMax();

	//skips last values if given time>MaxTime
	BenchScenarioParam(std::string Name,float MaxTime=-1);
	virtual ~BenchScenarioParam();
};

#endif /* SRC_BENCHSCENARIOPARAM_H_ */
