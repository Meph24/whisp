/*
 * BenchmarkTest.h
 *
 *  Created on:	01.10.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHMARKTEST_H_
#define SRC_BENCHMARKTEST_H_


#include <vector>
#include <iostream>

class AABB;
class Entity;

class BenchmarkTest
{
public:
	std::vector<AABB *> bbs;
	void registerEntity(Entity * e);
	void reset();
	BenchmarkTest();
	virtual ~BenchmarkTest();
};
std::ostream& operator<< (std::ostream& os, const BenchmarkTest& bt);

#endif /* SRC_BENCHMARKTEST_H_ */
