/*
 * BenchResult.h
 *
 *  Created on:	12.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHRESULT_H_
#define SRC_BENCHRESULT_H_

#include <vector>

class BenchResult
{
public:
	float min=-1;
	float max=-1;
	float avg=-1;
	float median=-1;

	BenchResult(std::vector<float> & rawData);//consumes the data in the vector and extracts results
	virtual ~BenchResult();
};

#endif /* SRC_BENCHRESULT_H_ */
